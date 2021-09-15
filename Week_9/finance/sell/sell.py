from sqlite3.dbapi2 import Connection
import sys
import os
from typing import Union, Tuple
sys.path.append(os.getcwd())

from helpers import get_qty, lookup, get_cash, write_shares, set_cash
from app_config import DB_ERROR, QUOTE_NOT_FOUND
    
def sell_shares(db: Connection, user_id: int, share_symb: str, qty: int) -> Tuple[float, str]: 
    """sell shares return tuple(cash, message)"""

    # get current stock price
    try:
        price: float = lookup(share_symb)["price"]
    except (AttributeError, NameError, KeyError):
        return (-1, QUOTE_NOT_FOUND)

    # get current stock qty
    cur_qty: Union[int,None] = get_qty(user_id, share_symb, db)
    if cur_qty is None:
        return (-1, f"get_qty: {DB_ERROR} or {QUOTE_NOT_FOUND}")

    # check if it enough to sell
    if cur_qty < qty:
        return(-1, f"You have not enough QTY. Current = {cur_qty}")
    
    # get current user cash
    cash: float = get_cash(user_id, db)

    # add record to shares with (-) in qty
    if write_shares(user_id, share_symb, qty * (-1), price, db) is None:
        return (-1, f"write_shares: {DB_ERROR}")
    
    # set new cash limit in the `users` table
    if set_cash(user_id, cash + price * qty, db) is None:
        return (-1, f"set_cash: {DB_ERROR}")
    
    db.commit()

    return (cash + price * qty, "")