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
Keep logged data in a single big file but take daily backups at midnight in a separate file.  
Keep currentTotal, but replace the forever total with a dailyTotal and reset both variables at midnight, forever total will be managed in spreadsheet  
At midnight reset of currentTotal count should be 0, save the actual figure in the main file as a EODerror or something, this will be minused from dailyTotal in spreadsheet.   
Log data hourly  
What about time drift? Is it significant? How do we reset it? Just plug it into a computer occasionally and sketch can do?     
How long before the SD fills up?    
Do we need a manual reset from web page if it goes wrong?     

#### Luke Woodbury 14th November 2025
