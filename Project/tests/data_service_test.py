import sys
import os
sys.path.append(os.path.join(sys.path[0], '..'))

import json
import pytest
from mock import patch
from data_service import get_strava_activities, read_strava
from app import Activities

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
    
    
