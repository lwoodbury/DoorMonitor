# Purpose:

Uses an IR gate to count the presence of people in a room, keeps a running total, and number of people currently in the room.   
Uses RTC to keep track of time between power cycles and logs the data to an SD card.  
Creates a WiFi Access point and serves a web page (index.html stored on the SD card) with function to view, download, or delete the data file. 
Also stores persistent variables for peopleCount and totalCount on the SD and reloads on startup in case of power cycle.   

# Current Hardware: 

Adafruit Feather Huzzah ESP32 main board  
Adafruit Adalogger Feather Wing (RTC and SD card shield)  
2x IR gate sensors, DFROBOT SEN0239  

# TODO: 

Do we clear the log and persistent vars every midnight and create a new file?   
Easier to load data into a spreadsheet if we keep the same file, maybe we have a daily total and just reset at midnight? More chance of losing whole file?  
What about time drift? How do we reset it? Just plug it into a computer occasionally and sketch can do?   
How long before the SD fills up?  
How often do we save data to SD? 
Do we need a manual reset from web page if it goes wrong?   

#### Luke Woodbury 11th November 2025
