#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "NarsLibraries.h"

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

unsigned long uptime;
unsigned long prev;

void setup() 
{
	Serial.begin(115200);
	LCD.init();
	LCD.setBacklight(255);
	LCD.print(x2i("AA"));
	SerialCom.connected = true;
}

void loop() 
{
	uptime = millis();
	SerialCom.send(24, 123);
}

void serialEvent()
{
	//SerialCom.onSerialEvent(&done, &special);
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