#include <Wire.h>
#include "NarsLibraries.h"


WireSearch wireSearch;
WireHost Client1;

void setup()
{
	Wire.begin();
	Client1.deviceProperties.address = 0x01;
	Serial.begin(1000000);
}

void loop() 
{
	delay(500);
	Client1.deviceProperties.data[15] = 123;
	Client1.sendData();
}