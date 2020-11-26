/*
 *
*/

#ifndef clock_h // Put these two lines at the top of your file.
#define clock_h // (Use a suitable name, usually based on the file name.)

// Place your main header code here.

#include "Arduino.h"
#include "WiFi.h"
#include "config.h"


void InitRTC();
bool CheckTime();
void UpdateTimeRestrictions(uint8_t start, uint8_t stop);
void CalibrateRTC();
void printLocalTime();
void GetNTPTime();

//=============================================================================
// Global Instance Declarations (Publicly Accessible)
//-----------------------------------------------------------------------------
//extern SensorsClass Sensors;

#endif // sensors_h
