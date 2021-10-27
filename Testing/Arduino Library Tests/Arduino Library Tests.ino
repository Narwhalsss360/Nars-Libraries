#include <LiquidCrystal_I2C.h>
#include "NarsLibraries.h"

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

unsigned long uptime;
unsigned long prev;

void setup() 
{
	Serial.begin(1000000);
	LCD.init();
	LCD.setBacklight(255);
	LCD.print("Test");
}

void loop() 
{
	uptime = millis();
}

void serialEvent()
{
	SerialCom.onSerialEvent(&done, &special);
	uptime = millis();
}

void done(unsigned int _register, unsigned long data)
{
	switch (_register)
	{
	case 1:

	default:
		break;
	}
	if (uptime - prev >= 80)
	{
		LCD.clear();
		LCD.print("R " + (String)_register);
		LCD.setCursor(0, 1);
		LCD.print("D " + (String)data);
	}
}

void special(unsigned int _register, String data)
{

}