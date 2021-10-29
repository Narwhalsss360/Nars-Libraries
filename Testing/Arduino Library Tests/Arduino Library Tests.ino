#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "NarsLibraries.h"

#define NETFRAMEWORK 1000000
#define MSVC 115200
#define SLOW 1000

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

unsigned long uptime;
unsigned long prev;

bool debug = true;
bool sendDebug = false;
bool slow = false;
unsigned long dataCounter = 0;
unsigned int registerCounter = 0;

void setup() 
{
	Serial.begin(MSVC);
	LCD.init();
	LCD.setBacklight(255);
	SerialCom.connected = true;
	LCD.print("INIT");
}

void loop() 
{
	if (slow && sendDebug)
	{
		delay(SLOW);
	}
	if (sendDebug)
	{
		if (dataCounter >= 4294967295)
		{
			dataCounter = 0;
		}
		else
		{
			dataCounter++;
		}
		if (registerCounter >= 65535)
		{
			registerCounter = 0;
		}
		else
		{
			registerCounter++;
		}
		SerialCom.sendSpecial(registerCounter, "My Special String");
		//SerialCom.send(registerCounter, dataCounter);
	}
}

void serialEvent()
{
	//SerialCom.onSerialEvent(&done, &special);
	String completeString;
	while (Serial.available())
	{
		char inChar = (char)Serial.read();
		completeString += inChar;
	}

	LCD.clear();
	LCD.print(completeString);
}

void done(unsigned int _register, unsigned long data)
{
	if (debug)
	{
		LCD.clear();

		LCD.setCursor(0, 0);
		LCD.print("R " + (String)_register);

		LCD.setCursor(0, 1);
		LCD.print("D " + (String)data);
	}
	else
	{
		LCD.clear();

		LCD.setCursor(0, 0);
		LCD.print((String)SerialCom.data[1] + " SURF");

		LCD.setCursor(0, 1);
		LCD.print((String)SerialCom.data[2] + " MSL");
	}
}

void special(unsigned int _register, String data)
{
	if (debug)
	{
		LCD.clear();

		LCD.setCursor(0, 0);
		LCD.print((String)_register);

		LCD.setCursor(0, 1);
		LCD.print("D " + (String)data);
	}
}