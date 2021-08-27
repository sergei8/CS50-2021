import sys
import os
sys.path.append(os.getcwd())

from typing import Union
from helpers import get_api_key, lookup
from requests.models import Response
from app_config import QUOTE_NOT_FOUND

import requests

def get_quote(share_symb: str) -> float:
    """return current quote for share or -1 if not found or error
    use API to IEX with API_KEY"""
    
    resp = lookup(share_symb)
    
    # check lookup result
    if resp is None:
        return -1.0
    else:
        return resp['price']

# if __name__ == "__main__":
#     get_quote("nflx")
