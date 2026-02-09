// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_PCF8523 rtc;

void setup () {
  Serial.begin(115200);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.start();


}

void loop () {
  DateTime now = rtc.now();
  int hour = now.hour();
  int minute = now.minute();
  Serial.print(hour);
  Serial.print(":");
  Serial.println(minute);
  delay(3000);
}
