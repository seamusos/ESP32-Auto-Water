/*
 *  Wifi Config for Adafruit IO
 *
*/

#ifndef sensors_h    // Put these two lines at the top of your file.
#define sensors_h    // (Use a suitable name, usually based on the file name.)

// Place your main header code here.

#include "Arduino.h"

class SensorClass
{
	public:
		SensorClass(void);


		int GetMoisture(int sensorPin);

	private:

};



//=============================================================================
// Global Instance Declarations (Publicly Accessible)
//-----------------------------------------------------------------------------
//extern SensorsClass Sensors;


#endif // sensors_h
