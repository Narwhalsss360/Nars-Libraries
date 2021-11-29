#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "NarsLibraries.h"

#define BAUD 1000000
#define SLOW 1000

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

PushTogle myButton = new PushToggle(3);

unsigned long uptime;
unsigned long prev;

bool debug = true;
bool sendDebug = false;
bool slow = false;

unsigned long registerg;
unsigned long datag;

void setup() 
{
	Serial.begin(BAUD);
	LCD.init();
	LCD.setBacklight(255);
	SerialCom.connected = true;
	LCD.clear();
	pinMode(3, INPUT_PULLUP);
	pinMode(4, INPUT_PULLUP);
	pinMode(5, INPUT_PULLUP);
	pinMode(6, INPUT_PULLUP);

	pinMode(3, INPUT);
	pinMode(3, INPUT_PULLUP);
	myButton.invert = true;
}

void loop() 
{
	SerialCom.data[1] = !digitalRead(3);
	SerialCom.data[2] = !digitalRead(4);
	SerialCom.data[3] = !digitalRead(5);
	SerialCom.data[4] = !digitalRead(6);
	SerialCom.sendDataRange(1, 4);
	SerialCom.setReady();
	myButton.read();

	if (myButton.output)
	{
		turnOnLight();
	}
}

void serialEvent()
{
	SerialCom.onSerialEvent(&done, &special);
}

void done(unsigned int _register, unsigned long data)
{
	registerg = _register;
	datag = data;
}

void special(unsigned int _register, String data)
{
	if (debug)
	{
		LCD.clear();

		LCD.setCursor(0, 0);
		LCD.print("R " + (String)_register);

		LCD.setCursor(0, 1);
		LCD.print("D " + (String)data);
	}
}