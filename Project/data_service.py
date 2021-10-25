# get activities from strava datastore by provided
# strava api and write it to json file, read from
# json file etc.

from os import error
from app_config import config, Activities, ACTIVITIES_FILE, \
    CLIENT_ID, CLIENT_SECRET, ACCESS_TOKEN, REFRESH_TOKEN
import requests
import json
from typing import Optional, Tuple
import dataclasses
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
from fastapi.responses import RedirectResponse
from stravalib.client import Client

REDIRECT_URL = 'http://localhost:8000/authorized'


def get_strava_activities(token) -> list[Activities]:
    '''return list of some items from strava datastore'''

    # read data from strava
    if len(data := _read_strava(token)) == 0:
        return []

    # convert data into list of activiites
    try:
        activities = [Activities(activity_type  =item["type"],
                                distance       =item["distance"],
                                moving_time    =item["moving_time"],
                                total_time     =item["elapsed_time"],
                                is_private     =item["private"],
                                date           =item["start_date"][:-2],
                                average_speed  =item["average_speed"],
                                max_speed      =item["max_speed"]) 
                  for item in data]
    except TypeError:
        return None
    
    return activities


def write_actvities_file(activities: list[Activities]) -> Optional[list]:
    """write activities list to json file
    return tuple with activities coun or (-1, error_msg)"""
    
    if len(activities) == 0:
        return(-1, "Activities is empty")
    
    with open ("activities.json", "w") as f:
        try:
            # write activities as one json-formated string
            f.write(json.dumps([dataclasses.asdict(x)
                    for x in activities], indent=2))
        except error: 
            return(-1, error)
    
    return (len(activities), '')

def read_activities_file(type=None, date_from=None, date_to=None, private=None) -> list[Activities]:
    """return list of saved in the file activities with passed criterias"""
    
    # create filters from args
    type_filter = "True" if not type else f"x.activity_type == '{type}'"
    print("****", type_filter)
    private_filter = "True" if not isinstance(private, bool) else f"x.is_private == {private}"
    date_filter = "True" if (date_from is None and date_to is None) else \
        f"'{date_from}' <=x.date[:10] <= '{date_to}'"
    
    # read json and convert it to list of activities
    activities: list[Activities] = json_to_activities(ACTIVITIES_FILE)[1]

    # accumulate list of filtered activities
    print(type_filter, private_filter, date_filter)
    activities = [x for x in activities if all([
        eval(type_filter), eval(private_filter), eval(date_filter)
        ])]
    
    return activities
 

def _read_strava(token) -> list:
    """read activities from strava
    """
    activites_url = "https://www.strava.com/api/v3/athlete/activities"
    header = {'Authorization': 'Bearer ' + token}
    data = requests.get(activites_url, headers=header).json()
    return data


def json_to_activities(json_file) -> Tuple[int, list[Activities]]:
    """convert data from json file to the list of activities
    return tuple (code, list)
    where code = -1 or records count
          list = activities or [] """
    
    try:
        with open(json_file) as f:
            activities = [Activities(**d) for d in json.load(f)]
    except error:
        return (-1, [])
    
    return (len(activities), activities)
    
