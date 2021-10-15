# FastAPI server set on localhost port 5000
# grab from strava and delivers access token
# to `app`

import time
import pickle
from fastapi import FastAPI
from fastapi.responses import RedirectResponse
from fastapi.encoders import jsonable_encoder
from stravalib.client import Client
import uvicorn
from app_config import CLIENT_ID, CLIENT_SECRET, REDIRECT_URL, \
    STREAMLIT_SERVER_PORT, PKL_FILE_NAME

app = FastAPI()
client = Client()


def save_object(obj:any, filename:str)->None:
    """save pickle-obect with current access-token"""
    with open(filename, 'wb') as output:
        pickle.dump(obj, output, pickle.HIGHEST_PROTOCOL)


def load_object(filename:str):
    """get and return pickle-obect with access token from file"""
    with open(filename, 'rb') as input:
        loaded_object = pickle.load(input)
        return loaded_object


def refresh_token()->None:
    """refresh clients `rights` with new access token"""
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


@app.get("/")
def read_root():
    """build authorize url and rediret to it"""
    authorize_url = client.authorization_url(
        client_id=CLIENT_ID, redirect_uri=REDIRECT_URL)
    return RedirectResponse(authorize_url)


@app.get("/authorized/")
def get_code(state=None, code=None, scope=None):
    """get from strava access token, save it to local file and
    redirect to streamlit page"""
    
    token_response = client.exchange_code_for_token(
        client_id=CLIENT_ID, client_secret=CLIENT_SECRET, code=code)
    access_token = token_response['access_token']
    refresh_token = token_response['refresh_token']
    expires_at = token_response['expires_at']
    client.access_token = access_token
    client.refresh_token = refresh_token
    client.token_expires_at = expires_at
    save_object(client, PKL_FILE_NAME)
    return RedirectResponse(f"http://localhost:{STREAMLIT_SERVER_PORT}")


try:
    client = load_object('client.pkl')
    if time.time() > client.token_expires_at:
        refresh_token()

except FileNotFoundError:
    pass

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
