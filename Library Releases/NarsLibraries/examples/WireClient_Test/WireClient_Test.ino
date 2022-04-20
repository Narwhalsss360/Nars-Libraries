#include <Wire.h>
#include "NarsLibraries.h"

#define ADDR 0x01
#define ID 1

//Client object.  ADDR  ID
WireClient client(ADDR, ID);

byte importantByte1;
byte importantByte2;
byte importantByte3;

void setup() 
{
	//Wire.
	Wire.begin(ADDR);

	//Adding callbacks
	Wire.onRequest(onRequest);
	Wire.onReceive(onReceive);

	//Debugging.
	Serial.begin(1000000);

	//Your data.
	importantByte1 = 16;
	importantByte2 = 64;
	importantByte3 = 255;

	//Getting data ready to send.
	client.deviceProperties.data[1] = importantByte1;
	client.deviceProperties.data[2] = importantByte1;
	client.deviceProperties.data[3] = importantByte1;
}

void loop() 
{
	
}

//Required for function.
void onRequest()
{
	client.onRequest();
}


//Required for function.
void onReceive(int bytes)
{
	client.onReceive(bytes);
}