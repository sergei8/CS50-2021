# main module of application `AgeFighter`
# run following modules:
#   - `data_service` - manipulate data for app
#   - `app_conig` - set config and shares for app

import dataclasses
from typing import Optional, Tuple

from pandas.core.frame import DataFrame
from app_config import config, AUTHORIZE_SERVER_PORT, PKL_FILE_NAME, Activities
from dataclasses import asdict, dataclass
import pickle
from data_service import get_strava_activities, write_actvities_file, read_activities_file
import streamlit as st
import time
import pandas as pd
from collections import namedtuple
from st_material_table import st_material_table
from datetime import datetime as dt

ReturnCode = namedtuple('ret_code', ["code", "message"])

def main():
    # hide streamlite marks
    hide_streamlit_style = read_markdown_file(
        './markdown/hide_streamlit_symbols.html')
    st.markdown(hide_streamlit_style, unsafe_allow_html=True)
    
    # set sidebar
    col1, col2, col3 = st.sidebar.columns([1, 2, 1])
    col2.image("./img/strava_logo.png", width=200, use_column_width='auto')

    view_about_style = read_markdown_file("./markdown/sidebar_buttons_style.html")
    col2.markdown(view_about_style, unsafe_allow_html=True)
    
    opt = col2.radio('', ["HOME", "VIEW STATISTIC", "ABOUT APP"], index=0)
    if opt == "VIEW STATISTIC":
        # get `token(code)` from `client.pkl`
        token = get_token()
            
        # get strava activities into `activities.json`
        data: Optional[list] = get_strava_activities(token)
        if data is None:
            return
        return_code = ReturnCode._make(write_actvities_file(data))
        if return_code.code == -1:
            st.write(return_code.message)
            print(return_code.messsage)
            return        
    
        # set menu row
        st.markdown("<h4 style='color: lightgreen;'>Filters:</h4>", unsafe_allow_html=True)
        type_menu, private_menu, date_from_menu, date_to_menu = st.columns(4)
        
        # set filters buttons
        with type_menu:
            ride = st.checkbox("Ride:", True)
            walk = st.checkbox("Walk:", True)
        
        with private_menu:
            private = st.checkbox("Private:", True)
            public  = st.checkbox("Public:", True)
            
            date_from = date_from_menu.date_input("Date From")
            date_to   = date_to_menu.date_input("Date To")
                    
        param = {
            "type"     : set_type_filter(ride, walk),
            "private"  : set_private_filter(private, public),
            "date_from": set_date_filter(date_from),
            "date_to"  : set_date_filter(date_to),
        }
        # view data activities
        view_data(param)
        
    elif opt == "ABOUT APP" :
        about_file =  read_markdown_file("./README.md")
        st.markdown(about_file)

    elif opt == "HOME":
        view_about()
        
def set_date_filter(date: any) -> Optional[str]:
    return None if not date else str(date)      
        
def set_private_filter(private: Optional[bool], public: Optional[bool]) -> Optional[bool]:
    if private and public:
        return None
    elif private:
        return True
    elif public:
        return False
    else:
        return False


def set_type_filter(ride: Optional[str], walk: Optional[str]) -> Optional[str]:
    if ride and walk:
        return None
    elif ride:
        return "Ride"
    elif walk:
        return "Walk"
    else:
        return "return nothing"

def get_token() -> Optional[str]:
    """get access token from saved strava tokens file """
    with open(PKL_FILE_NAME, 'rb') as f:
        athlete = pickle.load(f)
    return athlete.access_token
    
    
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
        ''            : 'Press `VIEW` button for look and analyse',
        "get_token"   : "Access token not found. Download it.",
        "token_expire": "Access token expired"
    }

    about_header = read_markdown_file("./markdown/about.html")
    st.markdown(about_header, unsafe_allow_html=True)
    st.image("./img/morda.png", use_column_width='auto')
    about_footer = f"<h5 style='color:gray; text-align:center'>{footer_message[status_message]}</h6>"
    st.markdown(about_footer, unsafe_allow_html=True)
    if status_message == "token_expire":
        menu_buttons = read_markdown_file("./markdown/download_button.html")
        st.markdown(menu_buttons, unsafe_allow_html=True)


def view_data(param={}):
    activities = pd.DataFrame([dataclasses.asdict(x) 
                               for x in read_activities_file(**param)])
    activities.rename({"activity_type": "ACTIVITY", "distance": "DISTANCE", "moving_time": "MOVING TIME",
                       "date" : "DATE", "average_speed": "AVG SPEED"}, inplace=True, axis=1)
    
    if not activities.empty:
        # format columns
        activities["DISTANCE"] = round(activities["DISTANCE"] / 1000, 1)
        activities["MOVING TIME"] = round(activities["MOVING TIME"] / 3600, 2)
        activities["DATE"] = activities["DATE"].astype(str)
        activities["DATE"] = activities["DATE"].map(lambda x: x[:10])
        activities["Avg Speed"] = round(activities["DISTANCE"] / activities["MOVING TIME"], 2)

        st.markdown("### ACTIVITIES DATA ###")
        _ = st_material_table(activities[["ACTIVITY", "DISTANCE", "MOVING TIME", "DATE", "AVG SPEED"]])



if __name__ == "__main__":
    main()
