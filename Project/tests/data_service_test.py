import sys
import os
sys.path.append(os.path.join(sys.path[0], '..'))

from app import Activities
from data_service import get_strava_activities, write_actvities_file, \
    read_activities_file
from mock import patch
import pytest
import json


@pytest.fixture
def mocked_activities():
    data = '''[
        {
            "resource_state": 2,
            "athlete": {
                "id": 1966015,
                "resource_state": 1
            },
            "name": "Ночная ходьба",
            "distance": 6556.8,
            "moving_time": 3971,
            "elapsed_time": 4210,
            "total_elevation_gain": 18.5,
            "type": "Walk",
            "id": 6008219662,
            "external_id": "344e7928-22ea-42ed-8909-8e1bb8d56aa5-activity.fit",
            "upload_id": 6385923558,
            "start_date": "2021-09-23T18:20:07Z",
            "start_date_local": "2021-09-23T21:20:07Z",
            "timezone": "(GMT+02:00) Europe/Kiev",
            "utc_offset": 10800,
            "start_latlng": [
                    50.52032,
                    30.510171
            ],
            "end_latlng": [
                50.520219,
                30.510893
            ],
            "location_city": null,
            "location_state": null,
            "location_country": "Украина",
            "start_latitude": 50.52032,
            "start_longitude": 30.510893,
            "achievement_count": 0,
            "kudos_count": 0,
            "comment_count": 0,
            "athlete_count": 1,
            "photo_count": 0,
            "map": {
                "id": "a6008219662",
                "resource_state": 2
            },
            "private": true,
            "average_speed": 1.598,
            "max_speed": 8.9

    }
    ]'''
    return json.loads(data)


def test_get_strava_activities(mocked_activities):
    """should return list of activities or [] if error"""

    with patch("data_service.read_strava") as read_strava:

        # check for presented activities
        read_strava.return_value = mocked_activities
        result = get_strava_activities()
        expected = [
            Activities("Walk", 6556.8, 3971, 4210, "2021-09-23T18:20:0",
                       True, 1.598, 8.9)
        ]
        assert result == expected

        # check for empty activities or error
        read_strava.return_value = []
        result = get_strava_activities()
        expected = []
        assert result == expected

def test_write_actvities_file():
    """should write mocked activities and return result tuple"""
    
    _activities = [
        Activities("Walk", 6556.8, 3971, 4210, "2021-09-23T18:20:0",
                       True, 1.598, 8.9)
    ]
    result = write_actvities_file(_activities)
    assert result == (1, '')

    assert write_actvities_file([])[0] == -1
    
    
@pytest.fixture
def mocked_activities_file():
    return [
         Activities("Walk", 6556.8, 3971, 4210, "2021-09-23T18:20:0",
                       True, 1.598, 8.9),
         Activities("Ride", 6556.8, 3971, 4210, "2021-09-24T18:20:0",
                       False, 1.598, 8.9),
         Activities("Walk", 6556.8, 3971, 4210, "2021-09-24T18:20:0",
                       True, 1.598, 8.9)
    ]
    
    
def test_read_activities_file(mocked_activities_file):
    """should read mocked file and return activities list"""
    
    with patch("data_service.json_to_activities") as json_to_activities:
        json_to_activities.return_value = mocked_activities_file
        result = read_activities_file(type="Ride")
        expected = mocked_activities_file
        assert result == expected
    
    
    
    
    