// HOST DEVICE

#include <Wire.h>
#include "NarsLibraries.h"

PROGMEM const byte button1Pin = 3;
PROGMEM const byte button2Pin = 5;
WireSearch wireSearch;
WireHost Client1;

unsigned long uptime,
prev;

void setup() 
{
	Wire.begin();
	Client1.deviceProperties.address = 0x01;
	Serial.begin(1000000);
	pinMode(button1Pin, INPUT_PULLUP);
	pinMode(button2Pin, INPUT_PULLUP);
}

void loop() 
{
	delay(500);
	Client1.deviceProperties.data[15] = 123;
	Client1.sendData();
}