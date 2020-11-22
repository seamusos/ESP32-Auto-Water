
#include <Arduino.h>
#include "sensors.h"

SensorClass::SensorClass(void)
{
  
}

int SensorClass::GetMoisture(int sensorPin)
{
  int sensorData = 0;

//  digitalWrite(SENSOR_POWER,HIGH); //Turn on Sensor
  delay(10);

  sensorData = analogRead(sensorPin);

//  digitalWrite(SENSOR_POWER, LOW); //Turn of Sensor

  return sensorData;
};
