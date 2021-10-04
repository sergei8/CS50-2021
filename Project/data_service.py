# get activities from strava datastore by provided
# strava api and write it to json file, read from
# json file etc.

from os import error
from app_config import config, Activities, ACTIVITIES_FILE, \
    CLIENT_ID, CLIENT_SECRET, ACCESS_TOKEN, REFRESH_TOKEN
import requests
import json
from typing import Tuple
import dataclasses
import requests
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

def get_strava_activities() -> list[Activities]:
    '''return list of some items from strava datastore'''

    # read data from strava
    if len(data := read_strava()) == 0:
        return []

    # convert data into list of activiites
    activities = [Activities(activity_type  =item["type"],
                             distance       =item["distance"],
                             moving_time    =item["moving_time"],
                             total_time     =item["elapsed_time"],
                             is_private     =item["private"],
                             date           =item["start_date"][:-2],
                             average_speed  =item["average_speed"],
                             max_speed      =item["max_speed"]) 
                  for item in data]

    return activities


def write_actvities_file(activities: list[Activities]) -> Tuple[int, str]:
    """write activities list to json file
    return tuple with activities coun or (-1, error_msg)"""
    
    if len(activities) == 0:
        return(-1, "Activities is empty")
    
    with open ("activities.json", "w") as f:
        for item in activities:
            try:
                f.write(json.dumps(dataclasses.asdict(item), indent=2))
            except error: 
                return(-1, error)
    return (len(activities), '')


def read_activities_file(type=None, date_from=None, date_to=None, private=None) -> list[Activities]:
    """return list of saved in the file activities with passed criterias"""
    
    # create filters from args
    type_filter = "True" if not type else f"x.activity_type == '{type}'"
    private_filter = "True" if not isinstance(private, bool) else f"x.is_private == {private}"
    date_filter = "True" if (date_from is None and date_to is None) else \
        f"'{date_from}' <=x.date[:10] <= '{date_to}'"
    
    # read json and convert it to list of activities
    activities: list[Activities] = json_to_activities()
    
    # accumulate list of filtered activities
    activities = [x for x in activities if all([
        eval(type_filter), eval(private_filter), eval(date_filter)
        ])]
    
    return activities
 

def read_strava() -> list:
    """read activities from strava. return tuple (code, list)
    code = -1 (error) or `activities count`
    list = records from api response
    """
    
    auth_url      = "https://www.strava.com/oauth/token"
    activites_url = "https://www.strava.com/api/v3/athlete/activities"
    
    payload = {
        'client_id': CLIENT_ID,
        'client_secret': CLIENT_SECRET,
        'refresh_token': REFRESH_TOKEN,
        'grant_type': "refresh_token",
        'f': 'json'
    }
    
    # get access token 
    res = requests.post(auth_url, data=payload, verify=False)
    # access_token = res.json()['access_token']
    access_token = "ccb46d5cc55d6346580e58de602fb2af92667491"

    # get data
    header = {'Authorization': 'Bearer ' + access_token}
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
    
