#include "SD.h"
#include "FS.h"

// File name where readings will be saved
const char* dataPath = "/data.txt";


// Init microSD card
void initSDCard(){
  if(!SD.begin()){
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

// Write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void updateVars(fs::FS &fs, const char * path){
  File file = fs.open(path, FILE_READ);
  if(!file) {
    Serial.println("Failed to open file for reading vars");
    return;
  }
  if (file) {
    while (file.available()) {
      file.read();

      peopleCount = file.parseInt();
      Serial.print("peopleCount: ");
      Serial.println(peopleCount);
      totalCount = file.parseInt();
      Serial.print("totalCount: ");
      Serial.println(totalCount);      

      Serial.println("Vars read");
      file.close();
    }

  }
  else {
    Serial.println("Read Error");
  }
  file.close();
}

// Delete file
void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\r\n", path);
  if(fs.remove(path)){
    Serial.println("- file deleted");
  } else {
    Serial.println("- delete failed");
  }
}