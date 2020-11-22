/* ###################################################################
**     Filename    : ESP32-AutomatedWatering.ino
**     Project     : 
**     Processor   : ESP32 Feather
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file ESP32-AutomatedWatering.ino
** @version 1.0
** @brief
**         Main module.
**         This module contains user's application code.
** @author Seamus O'Sullivan
** @date 31/12/2020
*/

/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "config.h" //Contains WiFi and Blynk Credientials, not included in this repo
#include <RTClib.h> //Adafruit RTC Library for PCF8532
#include "sensors.h"
//#include <WidgetRTC.h>
#include "time.h"
#include "clock.h"

// Your WiFi credentials.
// Set password to "" for open networks.


const int SENSOR_PIN = 32;
const int TAP_PIN = 14; //14 for DevKit
const int LED_PIN = 13;
const int SENSOR_POWER = 33;

// Blynk Virtual Pins
#define MoistureSensor_Vpin V0    //Output
#define WateringTimeLimit_Vpin V1 //Input
#define TimeRestrictions_Vpin V2
#define TimeRestrictionStop_Vpin V3
#define RSSI_Vpin V5
#define TapLED_Vpin V6
#define CalibrateRTC_Vpin V7

int UPDATE_INTERVAL = 10000;
int WateringTimeLimit = 900000;

//Defines State of Watering
enum Tap_State
{
  TAP_OFF = 0,
  TAP_ON = 1,
  MANUAL_OVERRIDE = 2
};

Tap_State TapState = TAP_OFF;

BlynkTimer timer;
SensorClass sensor;

void setup()
{

  // Debug console
  Serial.begin(115200);
  //while (!Serial);
  delay(2000);

  pinMode(SENSOR_PIN, INPUT);
  pinMode(TAP_PIN, OUTPUT);

//  WiFi.smartConfigDone();
//    WiFi.beginSmartConfig();

  GetNTPTime();

  InitRTC();

  Blynk.begin(blynkauth, ssid, pass, serverIP, 8080);

  timer.setInterval(UPDATE_INTERVAL, theTimer);
}

void loop()
{
  Blynk.run();
  timer.run();
}

//Timer Called Every Update Interval
void theTimer()
{
  int moistureLevel = sensor.GetMoisture(SENSOR_PIN);

  Serial.println();
  Serial.print("Mositure Reading is = ");
  Serial.print(moistureLevel, DEC);
  Serial.println();

  Blynk.virtualWrite(MoistureSensor_Vpin, moistureLevel);

  if (TapState == TAP_OFF)
  {
    if (CheckTime())
    {
      if (moistureLevel > 1000)
      {
        //Turn on Tap
        Serial.println("Soil is dry, turning on Tap");
        startWateringTimed(WateringTimeLimit);
      }
    }
  }

  int RSSI = WiFi.RSSI();

  Serial.print("WiFi Signal Strength> ");
  Serial.println(RSSI);

  Blynk.virtualWrite(RSSI_Vpin, RSSI);
}

BLYNK_WRITE(WateringTimeLimit_Vpin)
{
  int wateringTimeMin;
  wateringTimeMin = param.asInt();
  WateringTimeLimit = wateringTimeMin * 60000;
  Serial.print("Watering Time Limit is> ");
  Serial.println(wateringTimeMin);
  Serial.print("In milliseconds> ");
  Serial.println(WateringTimeLimit);
};

BLYNK_WRITE(CalibrateRTC_Vpin)
{
  if (param.asInt() == 1)
  {
    CalibrateRTC();
  }
};

BLYNK_WRITE(TimeRestrictions_Vpin)
{
  Serial.println("Updateing Time");

  long StartTime = param[0].asLong();
  StartTime = StartTime / 3600;

  Serial.print("Start Time is> ");
  Serial.println(StartTime);

  long StopTime = param[1].asLong();
  StopTime = StopTime / 3600;
  Serial.print("Stop Time is> ");
  Serial.println(StopTime);

  UpdateTimeRestrictions(StartTime, StopTime);
};

BLYNK_CONNECTED()
{
  Serial.println("Blynk Connected");
  Blynk.syncAll();
};

void startWateringTimed(uint32_t wateringTime)
{
  digitalWrite(TAP_PIN, HIGH); //Turn on Solinoid
  digitalWrite(LED_PIN, HIGH); //Status LED
  TapState = TAP_ON;
  //Blynk.virtualWrite(TapLED_Vpin, );

  int success;

  success = timer.setTimeout(WateringTimeLimit, stopWatering);

  Serial.print("Success Value is: ");
  Serial.println(success);

  Serial.print("Tap Turned On for ");
  Serial.print(WateringTimeLimit / 60000);
  Serial.println(" min");
};

void stopWatering()
{
  Serial.println("Timer Finished, Turning off Tap \n");
  digitalWrite(TAP_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  TapState = TAP_OFF;
};
