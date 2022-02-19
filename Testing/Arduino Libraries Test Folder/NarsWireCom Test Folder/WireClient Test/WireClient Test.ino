//CLIENT DEVICE

#include <Wire.h>
#include "NarsLibraries.h"

byte reg = 70;
byte dat = 53;

WireClient client;

unsigned long uptime,
prev;


void setup() 
{
	Wire.begin(0x01);
	client.deviceProperties.adress = 0x01;
	client.deviceProperties.id = 1;
	Wire.onRequest(onRequest);
	Wire.onReceive(onReceive);
	Serial.begin(1000000);
}

void loop() 
{
	uptime = millis();
	if (uptime - prev >= 1000)
	{
		prev = uptime;

		Serial.print("Register: ");
		Serial.print(reg);
		Serial.print(" Data: ");
		Serial.println(client.deviceProperties.data[reg]);
	}
}

void onRequest()
{
	client.onRequest();
	Serial.println("Requested register: " + client.registerSelect);
}

void onReceive(int bytes)
{
	client.onReceive(bytes);
	Serial.println("Selected Register: " + client.registerSelect);
}