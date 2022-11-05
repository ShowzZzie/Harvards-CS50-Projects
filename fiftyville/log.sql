-- Keep a log of any SQL queries you execute as you solve the mystery.

-- first off: schema
sqlite3 fiftyville.db
.schema

-- Check content of Crime_Scene_Reports Table
SELECT * FROM crime_scene_reports
-- The crime took place on 28.07.2021 on Humphrey Street at 10:15am. It happened at a bakery. Three witness, all mention bakery

-- Check interviews with witnesses
SELECT * FROM interviews
-- Witnesses: Ruth, Eugene, Raymond
-- Ruth says that the thief drove away within 10 minutes of the theft. I should check security footage for cars that drove away from bakery's
-- parking lot between 10:15am and 10:25am

-- Eugene says she recognized the thief but doesn't know his name. Earlier that morning she saw the thief at Leggett Street withdrawing money
-- from ATM machine. I should check the atm_transactions table.

-- Raymond claims that thief was on the phone as he was leaving the bakery and said call lasted less than a minute. During the call thief
-- mentioned that he wants to take the earliest flight out of Fiftyville the following day (29.07.2021). The thief asked his friend to book
-- the ticket

-- Check the car in Bakery Sec Logs
SELECT * FROM bakery_security_logs
-- Cars that match Ruth's story:
-- 10:16am, exit, 5P2BI95  | Entered at: 9:15am
-- 10:18am, exit, 94KL13X  | Entered at: 8:23am
-- 10:18am, exit, 6P58WS2  | Entered at: 9:20am
-- 10:19am, exit, 4328GD8  | Entered at: 9:14am
-- 10:20am, exit, G412CB7  | Entered at: 9:28am
-- 10:21am, exit, L93JTIZ  | Entered at: 8:18am
-- 10:23am, exit, 322W7JE  | Entered at: 8:36am
-- 10:23am, exit, 0NTHK55  | Entered at: 8:42am
-- Check for when each car entered:
SELECT * FROM bakery_security_logs WHERE license_plate IN ("5P2BI95", "94KL13X", "6P58WS2", "4328GD8", "G412CB7", "L93JTIZ", "322W7JE", "0NTHK55")

-- time to check atm_transactions from the morning of 28.07.2021
SELECT * FROM atm_transanctions WHERE day = 28 AND atm_location = "Leggett Street"
-- ATM transactions from Legget Street:
-- 28500762 - withdraw, 48
-- 28296815 - withdraw, 20
-- 76054385 - withdraw, 60
-- 49610011 - withdraw, 50
-- 16153065 - withdraw, 80
-- 86363979 - deposit, 10
-- 25506511 - withdraw, 20
-- 81061156 - withdraw, 30
-- 26013199 - withdraw 35
-- one deposit, could it be to pay for tickets? Could tickets cost $10?
-- now I am able to check account numbers from atm_transactions to check them with bank accounts and get names
SELECT * FROM bank_accounts WHERE account_number IN ("28500762", "28296815", "76054385", "49610011", "16153065", "86363979", "25506511", "81061156", "26013199")
-- 28500762 - person_id: 467400  | 2014 - creation year
-- 28296815 - person_id: 395717  | 2014
-- 76054385 - person_id: 449774  | 2015
-- 49610011 - person_id: 686048  | 2010
-- 16153065 - person_id: 458378  | 2012
-- 86363979 - person_id: 948985  | 2010
-- 25506511 - person_id: 396669  | 2014
-- 81061156 - person_id: 438727  | 2018
-- 26013199 - person_id: 514354  | 2012
-- well let's check person_ids
SELECT * FROM people WHERE id IN ("467400", "395717", "449774", "686048", "458378", "948985", "396669", "438727", "514354")
-- Personal infos: id, name, phone number, passport number, license plate
-- 467400 - Luca    | (389) 555-5198 | 8496433585      | 4328GD8
-- 395717 - Kenny   | (826) 555-1652 | 9878712108      | 30G67EN
-- 449774 - Taylor  | (286) 555-6063 | 1988161715      | 1106N58
-- 686048 - Bruce   | (367) 555-5533 | 5773159633      | 94KL13X
-- 458378 - Brooke  | (122) 555-4581 | 4408372428      | QX4YZN3
-- 948985 - Kaelyn  | (098) 555-1164 | 8304650265      | I449449
-- 396669 - Iman    | (829) 555-5269 | 7049073643      | L93JTIZ
-- 438727 - Benista | (338) 555-6650 | 9586786673      | 8X428L0
-- 514354 - Diana   | (770) 555-1861 | 3592750733      | 322W7JE
-- Since this table contains license_plates, let's check if any of them match the plates on cars which left the bakery on 28th
SELECT * FROM people WHERE id IN ("467400", "395717", "449774", "686048", "458378", "948985", "396669", "438727", "514354") AND license_plate IN ("5P2BI95", "94KL13X", "6P58WS2", "4328GD8", "G412CB7", "L93JTIZ", "322W7JE", "0NTHK55")
-- Narrowing down: id, name, phone number, passport number, license plate
-- 467400 - Luca    | (389) 555-5198 | 8496433585      | 4328GD8
-- 686048 - Bruce   | (367) 555-5533 | 5773159633      | 94KL13X
-- 396669 - Iman    | (829) 555-5269 | 7049073643      | L93JTIZ
-- 514354 - Diana   | (770) 555-1861 | 3592750733      | 322W7JE

-- so our suspects are Luca, Bruce, Iman, Diana

-- let's look at Raymond's story and check flights
-- Two things to look at here: Raymond claims the phone lasted for less than a minute; apparently thief wanted the earliest flight out of
-- Fiftyville the next day, the 29th of July
-- Let's look at phone calls, however let's narrow down calls to outgoing from four suspects above
SELECT * FROM phone_calls WHERE caller IN ("(389) 555-5198", "(367) 555-5533", "(829) 555-5269", "(770) 555-1861") AND day = 28
-- so two calls lasted less than a minute, one lasted 75sec perhaps we should check on it later as Raymond might have underestimated the duration
-- for now let's focus on two with duration of less than minute:
--     caller          receiver      year   month   day   duration
-- (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45        --- Bruce called Robin
-- (770) 555-1861 | (725) 555-3243 | 2021 | 7     | 28  | 49        --- Diana called Philip
-- keep in mind:
-- (367) 555-5533 | (704) 555-5790 | 2021 | 7     | 28  | 75        --- Bruce called Carl
-- let's check who Bruce and Diana called that day
SELECT * FROM people WHERE phone_number IN ("(375) 555-8161", "(725) 555-3243", "(704) 555-5790")
-- Let's write down info about our receivers:
--   id      name     phone_number    passport_number   license_plate
-- 864400 | Robin  | (375) 555-8161 |                 | 4V16VO0
-- 847116 | Philip | (725) 555-3243 | 3391710505      | GW362R6
-- 652398 | Carl   | (704) 555-5790 | 7771405611      | 81MZ921

-- i guess it's hight time to check whether Robin, Philip or Carl booked any tickets recently
-- I'll check for passport numbers of Philip, Carl but also suspects - Diana and Bruce
SELECT * FROM passengers WHERE passport_number IN ("3391710505", "7771405611", "5773159633", "3592750733")
-- +-----------+-----------------+------+
-- | flight_id | passport_number | seat |
-- +-----------+-----------------+------+
-- | 10        | 3391710505      | 2A   |  -  Philip
-- | 10        | 7771405611      | 6C   |  -  Carl
-- | 18        | 3592750733      | 4C   |  -  Diana
-- | 19        | 7771405611      | 9D   |  -  Carl
-- | 24        | 3592750733      | 2C   |  -  Diana
-- | 28        | 3391710505      | 2A   |  -  Philip
-- | 36        | 5773159633      | 4A   |  -  Bruce
-- | 47        | 3391710505      | 4D   |  -  Philip
-- | 47        | 7771405611      | 8D   |  -  Carl
-- | 54        | 3592750733      | 6C   |  -  Diana
-- +-----------+-----------------+------+
-- Got our flight ids let's check'em also why did Bruce only fly once? sus
SELECT * FROM flights WHERE id IN (10, 18, 19, 24, 28, 36, 47, 54)
-- Info:
-- Flight 10: from 8 to 4 on 30th X
-- Flight 18: from 8 to 6 on 29th OKAY
-- Flight 19: from 2 to 8 on 28th X
-- Flight 24: from 7 to 8 on 30th X
-- Flight 28: form 3 to 8 on 26th X
-- Flight 36: from 8 to 4 on 29th OKAY
-- Flight 47: from 4 to 8 on 30th X
-- Flight 54: from 8 to 5 on 30th X
-- so two flights match Raymond's info: Flight 18 and Flight 36
-- Passnegers:
-- Flight 18: Diana
-- Flight 36: Bruce
-- nice one David
-- however however however Raymond said that suspect wants the earliest flight out of Fiftyville and since both Bruce and Diana
-- took off from the same airport (id: 8) then it means...

-- BRUCE IS THE THIEF
-- HIS ACCOMPLICE IS ROBIN
-- AND HE ESCAPED TO:
SELECT * FROM airports WHERE id = 4
-- AND HE ESCAPED TO: LAGUARDIA AIRPORT