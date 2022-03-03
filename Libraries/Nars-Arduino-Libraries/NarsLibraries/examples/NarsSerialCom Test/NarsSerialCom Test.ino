#include "NarsLibraries.h"

#define BAUD 1000000

void setup()
{
	Serial.begin(BAUD);
	SerialCom.connected = true;
	pinMode(3, INPUT);
	pinMode(4, INPUT);
	pinMode(5, INPUT);
	pinMode(6, INPUT);
}

void loop()
{
	//Get data ready to send.
	SerialCom.data[1] = digitalRead(3);
	SerialCom.data[2] = digitalRead(4);
	SerialCom.data[3] = digitalRead(5);
	SerialCom.data[4] = digitalRead(6);

	//Send data from register 1 to 4, 0 not avaible, reserved for 'Ready To Receive'
	SerialCom.sendDataRange(1, 4);

	//Set ready only once done doing all functions.
	SerialCom.setReady();
}

void serialEvent()
{
	//Functionality.
	SerialCom.onSerialEvent(&done, &special);

	//If you dont need callback, then use...
	//SerialCom.onSerialEvent(NULL, NULL);
}

void done(unsigned int _register, unsigned long data)
{
	if (_register == 2)
	{
		//Received 'data' at register 2.
	}
}

void special(unsigned int _register, String data)
{
	if (_register == 2)
	{
		//Received 'data (A String)' at register 2.
	}
}