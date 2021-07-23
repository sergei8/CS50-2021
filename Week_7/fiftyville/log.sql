-- Keep a log of any SQL queries you execute as you solve the mystery.

-- Find the crime record based on crime description
SELECT id, year, month, day, street, description
FROM crime_scene_reports
WHERE 
year == 2020 AND month == 7 AND day == 28
AND street == "Chamberlin Street"
AND description LIKE '%CS50%'

-- Find witnesses of the crime base on data and location
SELECT * FROM interviews
WHERE year == 2020 AND month == 7 AND day == 28 AND transcript LIKE '%courthouse%'

-- Based on witness `Ruth` find list of cars exit from parking in the crime time
SELECT * FROM courthouse_security_logs
WHERE year == 2020 AND month == 7 AND day == 28
AND hour == 10 AND (minute > 15 AND minute < 25)
AND activity == "exit"

-- Based on witness `Egene` find list of accounts which withdraw cash in ATM on `Fifer Street` at crime date
SELECT * FROM atm_transactions
WHERE year == 2020 AND month == 7 and day == 28
AND atm_location == "Fifer Street" AND transaction_type == "withdraw"

-- Based on witness `Raymond` find the earliest flight at next to crime date
SELECT * FROM flights
WHERE year == 2020 AND month == 7 AND day == 29
ORDER BY hour, minute
LIMIT 1

-- Based on witness `Raymond` find the list of calls at the crime date
SELECT * FROM phone_calls 
WHERE year ==2020 AND month == 7 AND day == 28 AND duration <= 60

-- Create list of callers from `people` and list of calls
SELECT people.license_plate, people.name, people.passport_number, people.phone_number 
FROM people JOIN _calls ON people.phone_number == _calls.caller

-- Create list of suspected thiefs passport number
SELECT _caller.passport_number FROM _caller 
JOIN 
    (SELECT passengers.flight_id as fid, passengers.passport_number  as pn FROM passengers
    JOIN _flight ON fid == _flight.id)
ON _caller.passport_number == pn

-- Enrich and shorten suspected thief list using bank account info
SELECT account_number, license_plate, passport_number FROM bank_accounts JOIN
    (SELECT people.id as pid, people.license_plate as license_plate , 
        people.passport_number AS passport_number FROM people JOIN _thief_suspect
    ON people.passport_number == _thief_suspect.passport_number)
ON bank_accounts.person_id == pid

-- Determine thief from suspect list and list of cars
SELECT people.* FROM people JOIN 
    (SELECT _suspecteds_details.license_plate AS lp FROM _suspecteds_details JOIN _cars
        ON _suspecteds_details.license_plate == _cars.license_plate)
ON people.license_plate  == lp

-- Based of flight info and table `airports` find destination city
SELECT airports.city FROM airports JOIN _flight 
ON airports.id == _flight.destination_airport_id

-- Find accomplice name based on the thief calls info
SELECT people.name FROM people JOIN 
    (SELECT _calls.receiver  FROM _calls JOIN __thief ON _calls.caller == __thief.phone_number)
ON people.phone_number == receiver
