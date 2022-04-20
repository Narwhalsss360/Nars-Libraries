#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "NarsLibraries.h"

void setup() 
{
	Serial.begin(SERIALCOM_BAUD);
	Serial.println(LIB_SIZE);
}

void loop() 
{
}