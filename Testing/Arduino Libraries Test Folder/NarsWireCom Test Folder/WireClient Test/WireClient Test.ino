//CLIENT DEVICE

#include <Wire.h>
#include "NarsLibraries.h"

byte reg = 70;
byte dat = 53;

WireClient client(0x01, 1);

unsigned long uptime,
prev;


void setup() 
{
	Wire.begin(0x01);
	Wire.onRequest(onRequest);
	Wire.onReceive(onReceive);
	Serial.begin(1000000);
}

void loop() 
{
	uptime = millis();
}

void onRequest()
{
	client.onRequest();
}

void onReceive(int bytes)
{
	client.onReceive(bytes);
}