# main module of application `AgeFighter`
# run following modules:
#   - `data_service` - manipulate data for app
#   - `app_conig` - set config and shares for app

from typing import Optional, Tuple

from pandas.core.frame import DataFrame
from app_config import config, AUTHORIZE_SERVER_PORT, PKL_FILE_NAME
from dataclasses import dataclass
import pickle
import data_service
import streamlit as st
import time
import pandas as pd


def set_page_layout():
    # hide streamlite marks
    hide_streamlit_style = read_markdown_file(
        './markdown/hide_streamlit_symbols.html')
    st.markdown(hide_streamlit_style, unsafe_allow_html=True)

    # set sidebar
    col1, col2, col3 = st.sidebar.columns([1, 2, 1])
    col2.image("./img/strava_logo.png", width=200, use_column_width='auto')

    menu_buttons = read_markdown_file("./markdown/token_button.html")
    col2.markdown(menu_buttons, unsafe_allow_html=True)
    
    view_about_style = read_markdown_file("./markdown/sidebar_buttons_style.html")
    col2.markdown(view_about_style, unsafe_allow_html=True)
    if col2.button("VIEW"):
        view_data()
    if col2.button("ABOUT"):
        view_about()

def check_file_status() -> Tuple[bool, any]:
    """check if access token file exists"""
    try:
        with open(PKL_FILE_NAME, 'rb') as f:
            athlete = pickle.load(f)
            return (True, athlete)
    except FileNotFoundError:
        return (False, '')


def check_token_expire(athlete) -> bool:
    """return true/false of """
    return time.time() > athlete.token_expires_at


def read_markdown_file(file_name: str) -> str:
    template = ''
    with open(file_name) as f:
        template = f.read()

    return template


def check_token_status(file_status) -> str:
    # if access token file not exists
    if not file_status[0]:
        return "get_token"
    else:
        if check_token_expire(file_status[1]):
            return "token_expire"
    return ''


def view_about():
    """set elements on the `about` page"""

    # get athlete file status
    file_status: tuple = check_file_status()

    # check access token status
    status_message: str = check_token_status(file_status)

    # footer message template
    footer_message = {
        '': 'Press `VIEW` button for look and analyse',
        "get_token": "Access token not found. Download it.",
        "token_expire": "Access token expired"
    }

    about_header = read_markdown_file("./markdown/about.html")
    st.markdown(about_header, unsafe_allow_html=True)
    st.image("./img/morda.png", use_column_width='auto')
    about_footer = f"<h5 style='color:gray; text-align:center'>{footer_message[status_message]}</h6>"
    st.markdown(about_footer, unsafe_allow_html=True)


def view_data():
    activities:DataFrame = pd.read_json("activities_full.json")
    st.dataframe(activities)
    # st.write("VIEW DATA")


def main():

    # prepare page layout
    set_page_layout()


if __name__ == "__main__":
    main()
