import sqlite3
db: sqlite3.Connection = None

API_KEY = "pk_4d3dbd5a59414701962fb108caf625a7"
API_LOCATION = "app" # env

NAME_EMPTY = "Empty Name"
PSW_EMPTY = "Empty password"
PSW_WEAK = "Weak password"
USER_PRESENT = "User name is already present"
DB_ERROR = "Error db connection"
NOT_ADDED = "User is not present"
USER_NOT_FOUND = "User is not found"
QUOTE_NOT_FOUND = "Not found"
NOT_LIMIT = "CASH IS NOT ENOUGH"