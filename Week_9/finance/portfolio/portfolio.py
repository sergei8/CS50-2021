from sqlite3.dbapi2 import Connection, Error
import sys
import os
from typing import Dict, List, Tuple, Union
sys.path.append(os.getcwd())

from dataclasses import dataclass
from functools import reduce
from helpers import get_cash
from app_config import DB_ERROR, USER_NOT_FOUND

def portfolio(user_id: int, db: Connection) -> Dict[str, Union[str, int, float]]: 
    """return info about user shares and balance"""
    
    # define row of portfolio
    @dataclass
    class Share:
        symbol: str
        company_name: str
        qty: int
        price: float
        total: float

    # define result portfolio
    @dataclass
    class User_state_info:
        shares:List[Share]
        cash: float
        total: float
    
    user_portfolio = User_state_info([], 0, 0)
        
    # get user cash from `users` table
    if cash := get_cash(user_id, db) is None:
        return [-1, f"`get_cash: {DB_ERROR} or {USER_NOT_FOUND}"]
    
    # fill result structure with cash
    user_portfolio.cash = cash
    
    # get user balance from `shares` table
    balance = get_shares_info(user_id, db)
    
    # fill result structure with `symbol` and `qty`
    reduce(lambda i, acc: acc.append(i), balance, user_portfolio.shares)
    
    
    # fill result structure
    return {}

def get_shares_info(user_id: int, db: Connection) -> List[Tuple[str, int]]: 
    """return qty balance of user stocks"""
    
    sql = f"""SELECT symbol, SUM(qty) FROM shares 
            WHERE user_id = {user_id}
            GROUP BY symbol;"""
            
    try:
        cur = db.cursor()
        cur.execute(sql)
        return cur.fetchall()
    except Error:
        return []
    
# TODO def fill_qty(balance: List[Tuple[str, int]]) -> List[] 