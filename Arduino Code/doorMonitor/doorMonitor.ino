/*
* Uses an IR gate to count people in a room
* Uses RTC to keep track of real time
* Logs the data to an SD card
*   - keeps a current total and a daily total as a single text file
*   - keeps a daily backup file and resets variables at midnight
* Creates a WiFi Access point and serves a web page with access to data file
* Also stores persistent variables for peopleCount and dailyCount on the SD
* and reloads on startup in case of powercycle
*
* TODO: 
Log data hourly
What about time drift? Is it significant? How do we reset it? Just plug it into a computer occasionally and sketch can do?
How long before the SD fills up?
Do we need a manual reset from web page if it goes wrong? 
Do we want a file explorer rather than a single dowload button on web page
*
* Luke Woodbury 14th November 2025
*/



#include <peopleCount.h>
void updatePeopleCount();

#include <WiFiNWeb.h>
void WiFisetup();
void webServerSetup();
/*
#include <SDcard.h>
void initSDCard();
void writeFile();
void appendFile();
void deleteFile();
*/
#include <RTCbitz.h>
void RTCsetup();

//Data logging string and persistent variables
String dataMessage;
String varStore;
String dailyBackupFilename;
String dailyBackupMessage;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(PIN_B1, INPUT);
  pinMode(PIN_B2, INPUT);

  RTCsetup();
  WiFisetup();
  initSDCard();
  webServerSetup();

  //update vars from SD in case of power cycle
  updateVars(SD, "/var.txt");

  delay(1000);
  Serial.println("Room counter started...");
}


void loop() {
  updatePeopleCount(); // update the counters first

  if ((millis() - lastTime) > timerDelay) {
      DateTime now = rtc.now();
      char* humanDay = daysOfTheWeek[now.dayOfTheWeek()];
      int hour = now.hour();
      int minute = now.minute();

      //humanTime = String(now.year(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.day(), DEC) + " " + String(humanDay);
      
      // Log the current and total counts and save persistent variables
      //dataMessage = String(humanTime) + ", Current: " + String(peopleCount) + ", Total: " + String(dailyCount) + "\r\n";
      dataMessage = now.timestamp(DateTime::TIMESTAMP_DATE) + " " + now.timestamp(DateTime::TIMESTAMP_TIME) + " " + String(humanDay) +  " Current: " + String(peopleCount) + " DailyTotal: " + String(dailyCount) + "\r\n";
      varStore = "Vars: " + String(peopleCount) + " " + String(dailyCount) + " ";
      dailyBackupFilename = "/" + now.timestamp(DateTime::TIMESTAMP_DATE) + "_backup";
      dailyBackupMessage = now.timestamp(DateTime::TIMESTAMP_DATE) + " " + now.timestamp(DateTime::TIMESTAMP_TIME) + " " + String(humanDay) +  " Current: " + String(peopleCount) + " DailyTotal: " + String(dailyCount) + "\r\n";

      Serial.print("Saving data: ");
      //Serial.println(dataMessage);

      //Append line to data log
      appendFile(SD, "/data.txt", dataMessage.c_str());

      //write over variable store
      Serial.print(varStore);
      writeFile(SD, "/var.txt", varStore.c_str());

  // Save a daily backup file at midnight and reset variables
  if (hour == 13 && minute > 00) {
    if (!midnightDoneToday) {
      midnightDoneToday = true;

      Serial.println("Running midnight job!");
      writeFile(SD, dailyBackupFilename.c_str(), dailyBackupMessage.c_str());
      peopleCount = 0;
      dailyCount = 0;

    }
  } else {
    midnightDoneToday = false;
  }

      lastTime = millis();
  }
}



