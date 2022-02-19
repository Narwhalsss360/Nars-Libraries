#include <Wire.h>
#include "NarsLibraries.h"

byte data[5];
WireHostLite client(0x01);

void setup() 
{
	Serial.begin(1000000);
	Wire.begin();
}

void loop() 
{
	delay(1000);
	client.check();
	Serial.println(client.getData(1));
}
