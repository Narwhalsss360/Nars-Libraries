#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "NarsLibraries.h"

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

unsigned long uptime;
unsigned long prev;

bool debug = true;

void setup() 
{
	Serial.begin(1000000);
	LCD.init();
	LCD.setBacklight(255);
	SerialCom.connected = true;
	LCD.print("INIT");
}

void loop() 
{

}

void serialEvent()
{
	SerialCom.onSerialEvent(&done, &special);
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