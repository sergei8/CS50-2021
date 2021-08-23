from register import register, _is_user_present

from app_config import db
from app_config import NAME_EMPTY, PSW_EMPTY, PSW_WEAK, USER_PRESENT, \
    DB_ERROR, NOT_ADDED

from mock import patch
import pytest
import sqlite3

@pytest.fixture()
def mock_users_table():
    """create inmemory db `users` and fill it with one record"""
    db = _make_users_db()
    user_record = ("Dick", "123qwe", 10000)
    db.cursor().execute("INSERT INTO users VALUES (NULL, ?, ?, ?);", user_record)
    db.commit()
    return db

@pytest.fixture()
def mock_bad_users_table():
    """create bad db comnnection"""
    return _make_broken_users_db()    

def test_empty_name():
    """should return tuple with users id=-1 and message"""
    assert register("", "123") == (-1, NAME_EMPTY)

def test_empty_psw():
    """should return tuple with users id=-1 and message"""
    assert register("Bob", "") == (-1, PSW_EMPTY)

def test_weak_psw():
    """should return tuple with users id=-1 and message"""
    assert register("Bob", "123") == (-1, PSW_WEAK)
    assert register("Bob", "1a3b") == (-1, PSW_WEAK)
    assert register("Bob", "123456789") == (-1, PSW_WEAK)
    assert register("Bob", "qwertyuopa") == (-1, PSW_WEAK)
    
def test_user_present(mock_users_table):
    """should return id=-1 and message if user present"""
    
    with patch('register.db', mock_users_table) as _:
        assert register("Dick", "qwer1234") == (-1, USER_PRESENT)

def test_broken_table(mock_bad_users_table):
    """should return id=-1 and message if error in db"""
    
    with patch('register.db', mock_bad_users_table) as _:
        assert register("Bob", "qwer1234") == (-1, DB_ERROR)
        
def test_is_user_present(mock_users_table, mock_bad_users_table ):
    """test fanction functionality"""
    
    with patch('register.db', mock_users_table ) as _:
        assert _is_user_present("Dick") == (True, (-1, USER_PRESENT))
    
    with patch('register.db', mock_users_table ) as _:
        assert _is_user_present("Bob") == (False, (-1, NOT_ADDED))
    
    with patch('register.db', mock_bad_users_table) as _:
        assert _is_user_present("Dick") == (False, (-1, DB_ERROR))

def test_register_Ok(mock_users_table):
    """should return id and name if user add success"""
    
    with patch('register.db',  mock_users_table) as _:
        result = register("Bob", "1234qwert")
        assert result[0] > 0
        assert result[1] == "Bob"
        

# helpers =======================

def _make_users_db():
    """create fake table in the memory"""
    
    db = sqlite3.connect(":memory:")
    cur = db.cursor()
    cur.execute("""CREATE TABLE users 
                (id INTEGER, 
                username TEXT NOT NULL, 
                hash TEXT NOT NULL, 
                cash NUMERIC NOT NULL DEFAULT 10000.00, 
                PRIMARY KEY(id))""")
    db.commit()
    return db

def _make_broken_users_db():
    """create broken table in the memory"""
    
    db = sqlite3.connect(":memory:")
    return db
