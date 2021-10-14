# main module of application `AgeFighter`
# run following modules:
#   - `data_service` - manipulate data for app
#   - `app_conig` - set config and shares for app

from typing import Optional
from app_config import config, AUTHORIZE_SERVER_PORT 
from dataclasses import dataclass
import data_service
import streamlit as st
# import pandas as pd

def set_page_layout():
    # hide streamlite marks
    hide_streamlit_style = """
            <style>
            #MainMenu {visibility: hidden;}
            footer {visibility: hidden;}
            .sidebar {background-color: red; }
            </style>
            """
    st.markdown(hide_streamlit_style, unsafe_allow_html=True) 
    
    # set sidebar
    st.sidebar.title("LOGO")
    st.sidebar.header("HEADER")
    st.sidebar.selectbox("prompt", [1,2,3])
    st.sidebar.button("Import")
    st.sidebar.markdown('<a href="http://127.0.0.1:8000">proba</a>', unsafe_allow_html=True)


def main():
    
    set_page_layout()
        

if __name__ == "__main__":
    main()
