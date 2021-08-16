from application import add_user, get_users

from mock import patch
import pytest
import sqlite3

@pytest.fixture()
def db_connection():
    return _make_fake_db()

@pytest.fixture()
def db_broken_connection():
    db = sqlite3.connect(":memory:")
    curs = db.cursor()
    return db

@pytest.fixture()
def filled_db_cnection():
    records = [
        ("bob", 1, 1),
        ("dik", 2, 2)
    ]
    db = _make_fake_db()
    curs = db.cursor()
    curs.executemany("INSERT INTO birthdays VALUES(NULL, ?, ?, ?);", records)
    db.commit()
    return db

def test_add_user(db_connection):
    """should return tuple of added user    """
    
    with patch('application.db', db_connection) as mock_db:
        assert add_user("Вася", 1, 10) == ("Вася", 1, 10)

def test_add_user_error(db_broken_connection):
    """should return () if error write db    """
    
    with patch('application.db', db_broken_connection) as mock_db:
        assert add_user("Вася", 1, 10) == ()
        
def test_get_users(filled_db_cnection):
    """should return all records from table"""
    
    with patch('application.db', filled_db_cnection) as mock_db:
        assert get_users() == [("bob", 1, 1), ("dik", 2, 2)]
        
def test_get_users_errors(db_broken_connection):
    """should return -1 if broken connection"""
    
    with patch('application.db', db_broken_connection) as mock_db:
        assert get_users() == -1
    
    

# ============= helpers =====================
def _make_fake_db():
    
    db = sqlite3.connect(":memory:")
    curs = db.cursor()
    curs.execute("""CREATE TABLE birthdays (
    id INTEGER,
    name TEXT,
    month INTEGER,
    day INTEGER,
    PRIMARY KEY(id))""")
    db.commit()
    return db
