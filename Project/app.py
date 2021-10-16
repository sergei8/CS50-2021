# main module of application `AgeFighter`
# run following modules:
#   - `data_service` - manipulate data for app
#   - `app_conig` - set config and shares for app

from typing import Optional, Tuple
from app_config import config, AUTHORIZE_SERVER_PORT, PKL_FILE_NAME 
from dataclasses import dataclass
import pickle
import data_service
import streamlit as st
import time
# import pandas as pd

def set_page_layout(layout_style=''):
    # hide streamlite marks
    hide_streamlit_style = read_markdown_file('./markdown/hide_streamlit_symbols.html')
    st.markdown(hide_streamlit_style, unsafe_allow_html=True) 
    
    # set sidebar
    col1, col2, col3 = st.sidebar.columns([1,2,1])
    col2.image("./img/strava_logo.png", width=200,use_column_width='auto')
    # st.sidebar.header("HEADER")
    # st.sidebar.selectbox("prompt", [1,2,3])
    # st.sidebar.markdown('<a href="http://127.0.0.1:8000">TOKEN</a>', unsafe_allow_html=True)
    
    buttons_style = read_markdown_file("./markdown/sidebar_buttons_style.html")
    st.sidebar.markdown(buttons_style, unsafe_allow_html=True)

    if layout_style == '':
        view_about()
    elif layout_style == "get_token":
        view_about("Access token not found. Download it.")
    elif layout_style == "token_expire":
        view_about()
        
    col2.button("TOKEN")
    col2.button("ACTIVITIES")
    col2.button("ABOUT")



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

def read_markdown_file(file_name:str)->str:
    template = ''
    with open(file_name) as f:
        template = f.read()
    
    return template
    
    
def view_about(message = ''):
    # st.header("ABOUT")
    about_header = read_markdown_file("./markdown/about.html")
    st.markdown(about_header, unsafe_allow_html=True)
    st.image("./img/morda.png", use_column_width='auto')
    about_footer = f"<h5 style='color:gray; text-align:center'>{message}</h6>"
    st.markdown(about_footer, unsafe_allow_html=True)   
def view_data():
    ...
         
def main():
    
    layout_style = ''
        
    # get athlete file status
    file_status: tuple = check_file_status()
    
    # if access token file not exists
    if not file_status[0]:
        layout_style = "get_token"
    else:
        if check_token_expire(file_status[1]):
            layout_style = "token_expire"
        
    # prepare page layout
    print(f"layout_style = {layout_style}")
    set_page_layout(layout_style)
 
    
    
        

if __name__ == "__main__":
    main()
