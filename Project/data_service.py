# get activities from strava datastore by provided
# strava api and write it to json file, read from 
# json file etc.

import requests
import json
# from dataclasses import dataclass
from app import Activities
    
    
def get_strava_activities() -> list[Activities]:
    '''return list of some items from strava datastore'''
    
    # read data from strava
    if len(data := read_strava()) == 0:
        return []
    
    # convert data into list of activiites
    activities = [Activities(activity_type=item["type"],
                            distance=item["distance"],
                            moving_time=item["moving_time"],
                            total_time=item["elapsed_time"],
                            is_private=item["private"],
                            date=item["start_date"][:-2],
                            average_speed=item["average_speed"],
                            max_speed=item["max_speed"]) for item in data]
    
    return activities

def write_actvities_file(activities: dict):
    ...
    
    
def read_activities_file(*args):
    ...
    
    
def read_strava()-> list:
    return []

    
        
