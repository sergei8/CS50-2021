import pytest
import sqlite3
from portfolio import get_shares_info

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
    
