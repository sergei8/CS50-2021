# VIEW AND ANALYZE SPORT ACTIVITIES
#### Video Demo: https://youtu.be/QkgMCBOsTQ0
#### Description:

App is intended for extract, display and analyze ride and walk activities based on data from Strava service.

App is a client-server architecure where server part is resonsible for authentication and deliver data from Strava by appropriate api and frontend using streamlit library to display sport activities

> Used program language: **Python**

> Used markdown languages: **HTML, CSS**

> Used frameworks: **FastApi**, **Streamlit**

> Used libraries: **Pandas**


##### App structure: #####

1. **app.py** - frontend module used for display display:
   - _home page_ - include short description and current status of app;
   - _about page_ - represent this info;
   - _view page_ - depict table with sport activities and filters panel;
2. **data_service.py** - backend module responses for connect and authenticate to Strava service, use some open api from strava;
3. **app_config.py** - configuration module includes common classes, constants etc.;
4. **.env** - file include secrets for strava api;
5. **/tests/data_service_test.py** - unit tests for beckend;
6. **/markdown/\*** - some html files with css style tags for stilish frontend;
7. **/img\*** - some image files (logo, etc.)

## app.py ## 

It is a main module executed as a frontend of application. It include next functions:

- __main__ - used for control and display html pages of app. It uses framework _Streamlit_ as a backend which generetes appropriate layouts for web pages, menues, controls etc.
- **\<prefix\>_filter** - few functions which set filters need for display result table with sport activities data
- __get_token__ - get access token from saved strava tokens file saved locally 
- **check_**\<postfix> - few functions need for check token, and local access file status
- __view_data__ - functions display page with strava activiti statistic and differen filters to play with data table
- __view_about__ - display this page (README.md file)

## data_service.py ##

Module response for data preparation, format and save

- __get_strava_activities__ - return list of some items from strava datastore by access token;
- __write_actvities_file__ - write activities list to json file return tuple with activities count or (-1, error_msg);
- __read_activities_file__ - return list of saved in the file activities with passed criterias;
- **_read_strava** - helper read activities from strava datastore;
- __json_to_activities__ - convert data from json file to the list of activities return tuple (code, list) where code = -1 or records count list = activities or [];

## api_server.py ##

Backend module repspnses for communicate with strava service using appropriate apis.
Uses framework [FastAPI](https://fastapi.tiangolo.com) and a lightning-fast ASGI server [Uvicorn](https://www.uvicorn.org)

Has two __get__ routes:
1. "/" - read clients attributes from local file, check access token expiration time if yes refresh token  if file not found - redirect to autorization;
2. "/authorized/" - get from strava access token, save it to local file and redirect to streamlit (frontend) page.

- __save_athlete_info__ - function save pickle-object with current access-token;
- __load_athlete_info__ - get and return pickle-object with access token from file;
- __refresh_token__ - refresh clients `rights` with new access token and save in local file.

