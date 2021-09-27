# main module of application `AgiFighter`
# run following modules:
#   - `data_service` - manipulate data for app
#   - 

from dataclasses import dataclass

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


def main():
    ...

if __name__ == "__main__":
    main()
