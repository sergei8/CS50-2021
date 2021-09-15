import os
from sqlite3.dbapi2 import Connection, Cursor, Error
from flask.wrappers import Request
import requests
import urllib.parse
from typing import List, Tuple, Union
from datetime import datetime, date, time
from functools import reduce, wraps
from operator import add
from flask import request

from flask import redirect, render_template, request, session
from app_config import API_KEY, API_LOCATION


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def lookup(symbol):
    """Look up quote for symbol."""

    # Contact API
    try:
        api_key = os.environ.get("API_KEY")
        url = f"https://cloud.iexapis.com/stable/stock/{urllib.parse.quote_plus(symbol)}/quote?token={api_key}"
        response = requests.get(url)
        response.raise_for_status()
    except requests.RequestException:
        return None

    # Parse response
    try:
        quote = response.json()
        return {
            "name": quote["companyName"],
            "price": float(quote["latestPrice"]),
            "symbol": quote["symbol"]
        }
    except (KeyError, TypeError, ValueError):
        return None


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"


def get_api_key() -> str:
    if API_LOCATION == "env":
        import os
        return os.environ.get("API_KEY")
    else:
        return API_KEY


def get_cash(user_id: int, db: Connection) -> Union[float, None]:
    """return cash field from user id record or None if not found"""
    cur: Cursor = db.cursor()
    sql: str = f"SELECT cash FROM users WHERE id = {user_id};"
    cur.execute(sql)
    cash = cur.fetchone()
    return cash[0] if cash else None


def write_shares(user_id: int, symb: str, qty: int, price: float, db: Connection) -> Union[int, None]:
    """add record to `shares` table return record id or None if error"""

    record = (user_id, symb, qty, price, datetime.isoformat(datetime.today()))
    try:
        cur = db.cursor()
        cur.execute(
            "INSERT INTO shares VALUES (NULL, ?, ?, ?, ?, ?);", record
        )
        return cur.lastrowid
    except Error:
        return None


def set_cash(user_id: int, new_cash: float, db: Connection) -> Union[int, None]:
    """insert cash int user table return new cash or None if error"""

    # insert new cash into `users` table
    try:
        cur = db.cursor()
        update_params = (new_cash, user_id)
        cur.execute(
            "UPDATE users SET cash = ? WHERE id = ?", update_params
        )
        return new_cash
    except Error:
        return None


def get_qty(user_id: int, symb: str, db: Connection):
    """return current shares qty of the user or None if error"""

    sql = f"SELECT qty FROM shares WHERE user_id ={user_id} AND symbol = '{symb}';"

    try:
        cur = db.cursor()
        cur.execute(sql)
        result: List[int] = [x[0] for x in cur.fetchall()]
        return sum(result) if len(result) > 0 else None
    except Error:
        return None
