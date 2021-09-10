import pytest
import sqlite3
from mock import patch
from dataclasses import asdict, dataclass, asdict
from portfolio import get_shares_info, fill_qty, get_company_and_price, \
    Share
from app_config import CMP_NOT_FOUND
import portfolio

@dataclass(order=True)
class Share:
    symbol: str
    company_name: str
    qty: int
    price: float
    total: float
    
@dataclass(order=True)
class User_state_info:
    shares: list[Share]
    cash: float
    total: float

@pytest.fixture
def mock_shares_table():
    db = sqlite3.connect(":memory:")
    cur = db.cursor()
    cur.execute("""CREATE TABLE shares 
                (id INTEGER, 
                user_id INTEGER, 
                symbol TEXT,
                qty INTEGER, 
                price NUMERIC, 
                date DATETIME,
                PRIMARY KEY(id))""")
    db.commit()
    records = [
        (10, "APPL",  10, 5.0, '2021-08-01T20:37:54.992456'),
        (12, "APPL",  10, 5.5, '2021-08-10T20:37:54.992456'),
        (11, "APPL",  10, 5.0, '2021-08-10T20:37:54.992456'),
        (10, "NFLX",  10, 5.0, '2021-08-31T20:37:54.992456'),
        (10, "APPL",  -5, 5.0, '2021-08-31T20:37:54.992456'),
    ]
    cur.executemany("INSERT INTO shares VALUES(NULL, ?, ?, ?, ?, ?)", records )
    db.commit()
    
    cur.execute("""CREATE TABLE users 
            (id INTEGER, 
            username TEXT NOT NULL, 
            hash TEXT NOT NULL, 
            cash NUMERIC NOT NULL DEFAULT 10000.00, 
            PRIMARY KEY(id))""")
    db.commit()
    user_record = (10, "Bob", "123qwe", 111.11)
    cur.execute("INSERT INTO users VALUES (?, ?, ?, ?);", user_record)
    
    db.commit()

    return db
    
def test_get_shares_info(mock_shares_table):
    result = get_shares_info(10, mock_shares_table)
    assert result == [
        ("APPL", 5),
        ("NFLX", 10)
    ]
    result = get_shares_info(11111, mock_shares_table)
    assert result == []
    
def test_fill_qty():
    result = fill_qty([("APPL", 5), ("NFLX", 10)])
    expected = [Share("APPL", "",  5, 0, 0), Share("NFLX", "", 10, 0, 0)]
    # compare two lists of datalasses as dict
    assert all(list(map(lambda x : asdict(x[0]) == asdict(x[1]), zip(result, expected))))


@pytest.fixture
def mock_lookup():
    def _result(symb):
        data = {"APPL": {"name": "Apple", "price": 5.7, "symbol": "APPL"},
                "NFLX": {"name": "NetFlix", "price": 5, "symbol": "NFLX"},
                " ": None}
        return data[symb]
    return _result

def test_get_company_and_price(mock_lookup):
    
    shares = [Share("APPL", "", 10, 0, 0),
              Share("NFLX", "", 11, 0, 0),
              Share(" ", "", 11111, 0, 0)]
    
    with patch("portfolio.lookup", mock_lookup):
    
        result = get_company_and_price(shares)
        assert result == [Share("APPL", "Apple", 10, 5.7, 0),
                            Share("NFLX", "NetFlix", 11, 5, 0),
                            Share(" ", CMP_NOT_FOUND, 11111, 0, 0)]
                        