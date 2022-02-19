// HOST DEVICE

#include <Wire.h>
#include "NarsLibraries.h"

WireHost Client1;

byte reg = 70;
byte dat = 53;

void setup() 
{
	Wire.begin();
	Client1.deviceProperties.address = 0x01;
	Client1.check();
	Serial.begin(1000000);
}

void loop() 
{
	Client1.check();
	if (Client1.deviceProperties.state == Client1.deviceProperties.states.CONNECTED)
	{
		Client1.deviceProperties.data[reg] = dat;
		Client1.sendData();
		Serial.print("Sent ");
		Serial.print(dat);
		Serial.print(" at register ");
		Serial.println(reg);
	}
	else
	{
		Serial.println("Not connected");
	}
}
