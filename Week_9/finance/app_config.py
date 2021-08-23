import sqlite3

db: sqlite3.Connection = None

NAME_EMPTY = "Empty Name"
PSW_EMPTY = "Empty password"
PSW_WEAK = "Weak password"
USER_PRESENT = "User name is already present"
DB_ERROR = "Error db connection"
NOT_ADDED = "User is not present"