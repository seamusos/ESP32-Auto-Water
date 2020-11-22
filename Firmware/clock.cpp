

#include <Arduino.h>
#include "clock.h"
#include "RTClib.h"
#include "WiFi.h"

RTC_PCF8523 rtc;

uint8_t RestrictionStartTime;
uint8_t RestrictionStopTime;

void InitRTC()
{
  //Configure RTC
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  GetNTPTime();

//  if (rtc.lostPower())
//  {
//    Serial.println("RTC lost power, lets set the time!");
//
//    CalibrateRTC();
//  }
  RestrictionStartTime = 10;
  RestrictionStopTime = 16;
};

bool CheckTime()
{
  //Read RTC
  DateTime currentTime = rtc.now(); /*!< currentTime - Stores current time from RTC*/
  //Serial Print time
  Serial.print("Time is:");
  Serial.print(currentTime.hour(), DEC);
  Serial.print(":");
  if (currentTime.minute() < 10)
  {
    Serial.print("0");
  }
  Serial.print(currentTime.minute(), DEC);
  Serial.println();

  if ((currentTime.hour() > RestrictionStartTime) && (currentTime.hour() < RestrictionStopTime))
  {
    /* code */
    Serial.println("Time is within restrictions");
    return 0;
  }
  else
  {
    Serial.println("Time is outside restrictions");
    return 1;
  }
};

void UpdateTimeRestrictions(uint8_t start, uint8_t stop)
{
  RestrictionStartTime = start;
  RestrictionStopTime = stop;
};

void CalibrateRTC()
{
  DateTime newTime;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }

  rtc.adjust(DateTime(timeinfo.tm_year,
                      timeinfo.tm_mon,
                      timeinfo.tm_mday,
                      timeinfo.tm_hour,
                      timeinfo.tm_min,
                      timeinfo.tm_sec));

  newTime = rtc.now();
  //Full Timestamp
  Serial.println(String("DateTime::TIMESTAMP_FULL:\t") + newTime.timestamp(DateTime::TIMESTAMP_FULL));
};

void GetNTPTime()
{
  const char *ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 36000;
  const int daylightOffset_sec = 0;

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
};

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
};
