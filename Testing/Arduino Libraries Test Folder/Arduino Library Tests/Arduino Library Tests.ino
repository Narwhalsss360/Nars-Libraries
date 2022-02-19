#include <ButtonDebounce.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "NarsLibraries.h"

#define BUTTON_PIN 4
#define INVERTED true
#define DEBOUNCE_DELAY 20
#define OUT LED_BUILTIN

PushToggle t1(BUTTON_PIN, INVERTED, DEBOUNCE_DELAY);

void setup()
{
	
}

void loop() 
{
	
}