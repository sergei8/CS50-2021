# VIEW AND ANALYZE SPORT ACTIVITIES
#### Video Demo:
#### Description:

App is intended for extract, display and analyze ride and walk activities based on data from Strava service.

App is a client-server architecure where server part is resonsible for authentication and deliver data from Strava by appropriate api and frontend using streamlit library to display sport activities

> Used program language: **Python**

> Used markdown languages: **HTML, CSS**

> Used frameworks: **FastApi**, **Streamlit**

> Used libraries: **Pandas**


##### App structure:#####

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