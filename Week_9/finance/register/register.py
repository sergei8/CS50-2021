import sys
import os
sys.path.append(os.getcwd())

from app_config import  db
from app_config import NAME_EMPTY, PSW_EMPTY, PSW_WEAK, USER_PRESENT, \
    DB_ERROR, NOT_ADDED

from werkzeug.security import generate_password_hash
from sqlite3 import Error as db_error
from sqlite3.dbapi2 import Cursor

def register(name: str, password: str) -> tuple[int, str]:
    """create user record in the table `uses`
    return tuple (id, name) if success or (-1, error message)"""
    
    # check correct name
    if len(name) == 0:
        return (-1, NAME_EMPTY)
    
    # check correct password
    if len(password) == 0:
        return (-1, PSW_EMPTY)
    if password.isalpha() or password.isnumeric() or len(password) < 8:
        return (-1, PSW_WEAK)
    
    # check if the user is already present in the db
    result = _is_user_present(name)
    if result[1] == (-1, DB_ERROR):
        return result[1]
    elif result[0] == True:
        return result[1]
    
    # create hash password
    hash_psw = generate_password_hash(password)
    
    # add user into db
    try:
        cur: Cursor = db.cursor().execute("INSERT INTO users VALUES (NULL, ?, ?, ? );", (name, hash_psw, 0))
    except db_error:
        return (-1, DB_ERROR)
    
    # remember user id
    id: int = cur.lastrowid
    
    # check if user added success
    result = _is_user_present(name)
    if result[0] != True:
        return result[1]
    
    db.commit()

    # finally return the sucsess result
    return (id, name)

def _is_user_present(name: str) -> tuple[bool, tuple[int, str]]:
    """Select user record from db `users`"""
    try:
        result = db.cursor().execute(f"""SELECT * FROM users WHERE username == "{name}";""")
    except db_error:
        return (False, (-1, DB_ERROR))
    
    if len(list(result)) != 0:
        return (True, (-1, USER_PRESENT))
    else:
        return (False, (-1, NOT_ADDED))
