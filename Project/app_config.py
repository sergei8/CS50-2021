from dotenv import dotenv_values
from dataclasses import dataclass

# get config from `.env` file
config = dotenv_values(".env")
CLIENT_ID = config["client_id"]
CLIENT_SECRET = config["client_secret"]
REFRESH_TOKEN = config["refresh_token"]
ACCESS_TOKEN = config["access_token"]

ACTIVITIES_FILE = "activities.json"

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