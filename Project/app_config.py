from dotenv import dotenv_values
from dataclasses import dataclass

# get config from `.env` file
config = dotenv_values(".env")
CLIENT_ID = config["client_id"]
CLIENT_SECRET = config["client_secret"]
REFRESH_TOKEN = config["refresh_token"]
ACCESS_TOKEN = config["access_token"]

ACTIVITIES_FILE = "activities.json"
AUTHORIZE_SERVER_PORT = 8000
STREAMLIT_SERVER_PORT = 8501
REDIRECT_URL = f'http://localhost:{AUTHORIZE_SERVER_PORT}/authorized'
PKL_FILE_NAME = "client.pkl"

@dataclass
class Activities:
    activity_type: str
    distance: float
    moving_time: int
    total_time:int
    date: str
    is_private: bool
    average_speed: float
    max_speed: float



# if __name__ == "__main__":
#     ...