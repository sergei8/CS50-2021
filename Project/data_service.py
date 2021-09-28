# get activities from strava datastore by provided
# strava api and write it to json file, read from
# json file etc.

from os import error
import requests
import json
from typing import Tuple
import dataclasses


# from dataclasses import dataclass
from app import Activities


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
    # private_filter = True if not private else private
    
    # read json and convert it to list of activities
    activities: list[Activities] = json_to_activities()
    
    # accumulate list of filtered activities
    activities = [x for x in activities if all([
        eval(type_filter)
        ])]
    
    
    return activities
 

def read_strava() -> list:
    ...


def json_to_activities() -> list[Activities]:
    ...
