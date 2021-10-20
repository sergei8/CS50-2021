# FastAPI server set on localhost port 5000
# grab from strava and delivers access token
# to `app`

import time
import pickle
from fastapi import FastAPI
from fastapi.responses import RedirectResponse
from stravalib.client import Client
import uvicorn
from app_config import CLIENT_ID, CLIENT_SECRET, REDIRECT_URL, \
    STREAMLIT_SERVER_PORT, PKL_FILE_NAME

app = FastAPI()
client = Client()


def save_athlete_info(obj: any, filename: str) -> None:
    """save pickle-obect with current access-token"""
    with open(filename, 'wb') as f:
        pickle.dump(obj, f, pickle.HIGHEST_PROTOCOL)


def load_athlete_info(filename:str):
    """get and return pickle-object with access token from file"""
    try:
        with open(filename, 'rb') as f:
            athlete = pickle.load(f)
            return athlete
    except FileNotFoundError:
        return None


def refresh_token(client) -> None:
    """refresh clients `rights` with new access token 
    and save in local file"""
    refresh_response = client.refresh_access_token(
        client_id=CLIENT_ID,
        client_secret=CLIENT_SECRET,
        refresh_token=client.refresh_token)
    access_token = refresh_response['access_token']
    refresh_token = refresh_response['refresh_token']
    expires_at = refresh_response['expires_at']
    client.access_token = access_token
    client.refresh_token = refresh_token
    client.token_expires_at = expires_at
    save_athlete_info(client, PKL_FILE_NAME)


@app.get("/")
def read_root():
    """read clients attributes from local file,
    check access token expiration time if yes 
    refresh token  if file not found - redirect to autorization """
    
    client = load_athlete_info(PKL_FILE_NAME)
    if client is None:
        client = Client()
        authorize_url = client.authorization_url(client_id=CLIENT_ID,
                                                 redirect_uri=REDIRECT_URL)
        return RedirectResponse(authorize_url)
        
    else:    
        if time.time() > client.token_expires_at:
            refresh_token(client)
        return RedirectResponse(f"http://localhost:{STREAMLIT_SERVER_PORT}")


@app.get("/authorized/")
def get_code(state=None, code=None, scope=None):
    """get from strava access token, save it to local file and
    redirect to streamlit page"""
    
    token_response = client.exchange_code_for_token(client_id=CLIENT_ID,
                                                    client_secret=CLIENT_SECRET,
                                                    code=code)
    access_token = token_response['access_token']
    refresh_token = token_response['refresh_token']
    expires_at = token_response['expires_at']
    client.access_token = access_token
    client.refresh_token = refresh_token
    client.token_expires_at = expires_at
    save_athlete_info(client, PKL_FILE_NAME)
    return RedirectResponse(f"http://localhost:{STREAMLIT_SERVER_PORT}")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
