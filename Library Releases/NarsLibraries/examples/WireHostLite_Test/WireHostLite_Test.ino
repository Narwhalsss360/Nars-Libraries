#include <Wire.h>
#include "NarsLibraries.h"

//Address of client
#define CLIENT_ADDR 0x01

//Things to save
byte importantByte1;
byte importantByte2;
byte importantByte3;
byte id;

byte counter;

//Client Object.
WireHostLite client(CLIENT_ADDR);

void setup() 
{
	//For debugging.
	Serial.begin(1000000);
	
	//Wire.
	Wire.begin();
}

void loop() 
{
	client.address = 0x01;
	
	//Things to calculate.
	if (counter < 255)
	{
		counter++;
	}
	else
	{
		counter = 0;
	}

	//Always checks if client device is available, will only automatically connect if this function runs.
	client.check();
	Serial.print ("Checking if client is available...	");
	
	delay(500);
	
	if (client.connected)
	{
		Serial.println("Connected.");
	}

	//This runs within check(), you cannot get data from address 0 as that stores the id YOU set in your client's code.
	//id = client.getData(0);

	//instead, use this, or just keep it in client.id.
	
	delay(500);

	id = client.id;

	Serial.print("Client ID:	");
	Serial.println(client.id);

	delay(500);

	//Get data from client, only 1-64 is available on both devices.
	importantByte1 = client.getData(1);
	importantByte2 = client.getData(2);
	importantByte3 = client.getData(3);

	Serial.print("ImportantByte1: ");
	Serial.println(importantByte1);
	Serial.print("ImportantByte2: ");
	Serial.println(importantByte2);
	Serial.print("ImportantByte3: ");
	Serial.println(importantByte3);

	//To change Address later
	client.address = 0x02;

	delay(500);

	//Send data to client, only 1-255 available on both sides
	client.sendData(65, counter);
	
	Serial.print("Sent:    ");
	Serial.println(counter);

	delay(1000);
}
