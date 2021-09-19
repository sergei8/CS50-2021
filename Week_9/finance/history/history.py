from sqlite3.dbapi2 import Connection, Error
import sys
import os
from typing import Tuple, List
sys.path.append(os.getcwd())

from helpers import get_qty, lookup, get_cash, write_shares, set_cash

def history(user_id: int, db: Connection) -> List[Tuple[str, int, float, str]]:
    """select from `shares` and return list of transactions by user id 
    or None if error"""

    # get rows from `shares`
    sql = f"SELECT * FROM shares WHERE user_id = {user_id};"
    try:
        cur = db.cursor()
        cur.execute(sql)
        result = cur.fetchall()
        return  [x[1:] for x in result] if result  else []
    except Error:
        return None
    