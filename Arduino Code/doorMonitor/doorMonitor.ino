/*
* Uses an IR gate to count people in a room
* Uses RTC to keep track of real time
* Logs the data to an SD card
* Creates a WiFi Access point and serves a web page with access to data file
* Also stores persistent variables for peopleCount and totalCount on the SD
* and reloads on startup in case of powercycle
*
* TODO: 
* Do we clear the log and persistent vars every midnight and create a new file? 
* Easier to load data into spreadsheet if we keep same file, maybe we have a daily total? 
*
* Luke Woodbury 11th November 2025
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
      humanDay = daysOfTheWeek[now.dayOfTheWeek()];

      //humanTime = String(now.year(), DEC) + "/" + String(now.month(), DEC) + "/" + String(now.day(), DEC) + " " + String(humanDay);
      
      // Log the current and total counts and save persistent variables
      //dataMessage = String(humanTime) + ", Current: " + String(peopleCount) + ", Total: " + String(totalCount) + "\r\n";
      dataMessage = now.timestamp(DateTime::TIMESTAMP_DATE) + " " + now.timestamp(DateTime::TIMESTAMP_TIME) + " " + String(humanDay) +  " Current: " + String(peopleCount) + " Total: " + String(totalCount) + "\r\n";
      varStore = "Vars: " + String(peopleCount) + " " + String(totalCount) + " ";
      
      Serial.print("Saving data: ");
      //Serial.println(dataMessage);

      //Append line to data log
      appendFile(SD, "/data.txt", dataMessage.c_str());

      //write over variable store
      Serial.print(varStore);
      writeFile(SD, "/var.txt", varStore.c_str());

      lastTime = millis();
  }
}





