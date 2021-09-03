import pytest
import sqlite3
from history import history

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
        (10, "APPL",  10, 5.5, '2021-08-10T20:37:54.992456'),
        (11, "APPL",  10, 5.0, '2021-08-10T20:37:54.992456'),
        (11, "NFLX",  10, 5.0, '2021-08-31T20:37:54.992456'),
        (10, "APPL", -10, 5.0, '2021-08-31T20:37:54.992456'),
    ]
    cur.executemany("INSERT INTO shares VALUES(NULL, ?, ?, ?, ?, ?)", records )
    db.commit()
    
    return db

def test_history(mock_shares_table):
    result = history(10, mock_shares_table)
    assert result == [
        (10, "APPL",  10, 5.0, '2021-08-01T20:37:54.992456'),
        (10, "APPL",  10, 5.5, '2021-08-10T20:37:54.992456'),
        (10, "APPL", -10, 5.0, '2021-08-31T20:37:54.992456'),
    ]
    result = history(1111, mock_shares_table)
    assert result == []

def test_history_bad_db():
    db = sqlite3.connect(":memory:")
    assert history(123, db) == None