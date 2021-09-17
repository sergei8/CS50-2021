from app_config import DB_ERROR, USER_NOT_FOUND, CMP_NOT_FOUND
from helpers import get_cash, lookup
from functools import reduce
from dataclasses import dataclass
from sqlite3.dbapi2 import Connection, Error
import sys
import os
from typing import Dict, List, Tuple, Union

sys.path.append(os.getcwd())


# define row of portfolio
@dataclass(order=True)
class Share:
    symbol: str
    company_name: str
    qty: int
    price: float
    total: float

# define result portfolio
@dataclass(order=True)
class User_state_info:
    shares: List[Share]
    cash: float
    total: float


def portfolio(user_id: int, db: Connection) -> Dict[str, Union[str, int, float]]:
    """return info about user shares and balance"""

    # accumulator
    user_portfolio = User_state_info([], 0, 0)

    # get user cash from `users` table
    if (cash := get_cash(user_id, db)) is None:
        return [-1, f"`get_cash: {DB_ERROR} or {USER_NOT_FOUND}"]

    # fill result structure with cash
    user_portfolio.cash = round(cash,2)

    # get user balance from `shares` table
    balance = get_shares_info(user_id, db)

    # fill result structure with `symbol` and `qty`
    user_portfolio.shares = fill_qty(balance)

    # fill result structure with 'company_name" and `price`
    user_portfolio.shares = get_company_and_price(user_portfolio.shares)

    # calc and fill `total` field
    user_portfolio.total = sum([x.total for x in user_portfolio.shares]) + user_portfolio.cash
    
    return user_portfolio.__dict__


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


def fill_qty(balance: List[Tuple[str, int]]) -> List[Share]:
    """return list of `shares` class filled with symbol and qty"""

    return reduce(lambda acc, i:
                  acc + [Share(qty=i[1], symbol=i[0],
                               company_name="", price=0, total=0)],
                  balance, [])


def get_company_and_price(shares: list[Share]) -> list[Share]:
    """return list of `shares` filled with company_name, and current stock prices"""

    for share in shares:

        share_info = lookup(share.symbol)
        if share_info is not None:
            share.company_name = share_info["name"]
            share.price = share_info["price"]
            share.total = share.price * share.qty
        else:
            share.company_name = CMP_NOT_FOUND

    return shares
