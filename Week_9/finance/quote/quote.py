import sys
import os
sys.path.append(os.getcwd())

from typing import Union
from helpers import get_api_key
from requests.models import Response
from app_config import QUOTE_NOT_FOUND

import requests

def get_quote(share_symb: str) -> Union[float, None]:
    """return current quote for share or -1 if not found or error
    use API to IEX with API_KEY"""
    
    # get API_KEY 
    api_key = get_api_key()
    
    # create api request stmt
    url = f"https://cloud.iexapis.com/stable/stock/{share_symb}/quote?token={api_key}"
    
    # run api
    try:
        resp:Response = requests.get(url)
    except:
        return -1
    
    # check api result
    if resp.status_code == 200:
        return resp.json()['latestPrice']
    else:
        return -1

if __name__ == "__main__":
    get_quote("nflx")
