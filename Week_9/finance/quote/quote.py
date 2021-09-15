import sys
import os
sys.path.append(os.getcwd())
from helpers import get_api_key, lookup

def get_quote(share_symb: str) -> float:
    """return current quote for share or -1 if not found or error
    use API to IEX with API_KEY"""
    
    resp = lookup(share_symb)
    
    # check lookup result
    if resp is None:
        return -1.0
    else:
        return resp['price']
