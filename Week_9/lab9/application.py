import os

# from cs50 import SQL
import sqlite3
from typing import Dict, Set, Tuple, Union, List
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = sqlite3.connect("birthdays.db", check_same_thread=False)
# db = SQL("sqlite:///birthdays.db")


def add_user(name: str, month: int, day: int) -> Union [Tuple[str, int, int], Tuple[()]]: 
    """Add user record into db and return tuple with user items or () if error"""
    
    # construct insert sql statement
    sql = "INSERT INTO birthdays VALUES (NULL, ?, ?, ? );"
    
    # insert user into the table
    try:
        db.cursor().execute(sql, (name, month, day))
        db.commit()
    except sqlite3.Error:
        return ()
    
    return (name, month, day)

def get_users() -> Union[List[Tuple[str, int, int]], int]:
    """return records from table `birthdays`"""
        
    # construct sql statement for select all rows
    sql = "SELECT * FROM birthdays;"
    
    # get records from table `birthdays`
    try:
        cur = db.cursor()
        cur.execute(sql)
    except sqlite3.Error:
        return -1
    
    # make return list of table records
    result = []
    for record in cur:
        result.append(record[1:]) # append items from 1 to end

    return result



@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":

        # get name, month, day  from request get
        name, month, day = request.form.values()
        
        # Add the user's entry into the database
        add_user(name, month, day)
            

        return redirect("/")

    else:

        # Display the entries in the database on index.html
        records: Union[List [Tuple[str, int, int]], int] = get_users()
        
        # return render_template("index.html", users = -1)
        return render_template("index.html", users=records)
        # return  jsonify(records)



if __name__ == "__main__":
    app.run(debug=True)