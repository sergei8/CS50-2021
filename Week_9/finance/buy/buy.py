from sqlite3.dbapi2 import Connection
import sys
import os
sys.path.append(os.getcwd())

from helpers import lookup, get_cash, write_shares
from app_config import DB_ERROR, USER_NOT_FOUND, \
    QUOTE_NOT_FOUND, NOT_LIMIT

def buy_share(db: Connection, user_id: int, share_symb: str, qty: int):
    """provide buy shares"""

    # get current stock price
    try:
        price: float = lookup(share_symb)["price"]
    except (AttributeError, NameError, KeyError):
        return (-1, QUOTE_NOT_FOUND)

    # get current user cash
    cash: float = get_cash(user_id, db)

    # check if user found and has enough cash limit
    if cash is None:
        return (-1, f"{DB_ERROR} or {USER_NOT_FOUND}")
    if (cash < price * qty):
        return (-1, f"{NOT_LIMIT} cash: {cash}, your request: {price * qty}")

    # add record to shares 
    if write_shares(user_id, qty, price, db) is None:
        return (-1, DB_ERROR)

    # correct cash limit
    
    
    db.commit()