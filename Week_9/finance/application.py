import os
from app_config import DB_ERROR, db

# from cs50 import SQL
import sqlite3 as SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from register.register import register as reg

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached    
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL.connect("finance.db", check_same_thread=False)
# db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # portfolio_table = portfolio()
    portfolio_table = dict()
    if not isinstance(portfolio_table, dict):        
        return apology("Error in portfolio")
    
    return render_template("portfolio.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    return apology("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not (user_name := request.form.get("username")):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not (password := request.form.get("password")):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(f"SELECT * FROM users WHERE username = '{user_name}'")
        users:list = list(rows)
        # Ensure username exists and password is correct
        if len(users) != 1 or not check_password_hash(users[0][2], password):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = users[0][0]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    return apology("TODO")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    if request.method == "POST":
        
        if not (usernamme := request.form.get("username")):
            return apology("must provide username", 403)
        if not (password := request.form.get("password")):
            return apology("must provide password", 403)
        if not (password_again := request.form.get("password_again")):
            return apology("must provide password again", 403)
        if password != password_again:
            return apology("passwords not equal", 403)
        
        user_id, msg = reg(usernamme, password, db)
        if user_id == -1:
            return apology(msg, 403)
        
        return redirect("/")
        
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


    
if __name__ == '__main__':
    app.run(host='127.0.0.1', port=8000, debug=True)
     