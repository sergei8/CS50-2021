import sqlite3
import pytest
from mock import patch

from helpers import get_cash, write_shares, set_cash, get_qty


@pytest.fixture
def mock_user_table():
    db = sqlite3.connect(":memory:")
    cur = db.cursor()
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
    return db

@pytest.fixture
def mock_shares_with_rows():
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
        (10, "APPL",  10, 5.0, '2021-08-10T20:37:54.992456'),
        (11, "APPL",  10, 5.0, '2021-08-10T20:37:54.992456'),
        (11, "NFLX",  10, 5.0, '2021-08-31T20:37:54.992456'),
        (10, "APPL", -10, 5.0, '2021-08-31T20:37:54.992456'),
    ]
    cur.executemany(
        "INSERT INTO shares VALUES(NULL, ?, ?, ?, ?, ?)", records
    )
    db.commit()
    
    return db

@pytest.fixture
def mock_table_bad():
    return sqlite3.connect(":memory:")


def test_get_cash(mock_user_table):
    """it should return cash from db for user id"""
    result = get_cash(10, mock_user_table)
    assert result == 111.11


def test_get_cash_bad(mock_user_table):
    """it should return None if user not found"""
    result = get_cash(11111, mock_user_table)
    assert result == None


def test_write_shares(mock_shares_table):
    """it should return positive number"""
    result = write_shares(10, "APPL", 1100, 55.5, mock_shares_table)
    assert result > 0


def test_write_shares_bad(mock_table_bad):
    """it should return None"""
    result = write_shares(10, "APPL", 1100, 55.5, mock_table_bad)
    assert result == None

def test_set_cash(mock_user_table):
    """it should return 9000 as new cash"""
    result = set_cash(1, 9000, mock_user_table)
    assert result == 9000
    
def test_get_qty(mock_shares_with_rows):
    """should return positive or 0"""
    result = get_qty(10, "APPL", mock_shares_with_rows)
    assert result == 10
    result = get_qty(10, "AAAA", mock_shares_with_rows)
    assert result == None