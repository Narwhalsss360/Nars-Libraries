#include <NarsLibraries.h>

#define BAUD 1000000

//Button pin.
int buttonPin = 3;

//Delay for debouncing.
int debounceDelay = 30;

//Push constructor, instanciating Push instance as button.
Push button(buttonPin, INPUT_PULLUP, debounceDelay);

void setup()
{
	//pinMode is already done at constructor.

	//Your onPress function will run when the button is pressed in.
	button.onPress = onPress;

	//Your onRelease function will run when the button is released.
	button.onRelease = onRelease;
}

void loop() 
{
	bool currentState;

	//Input reading, and logic for on press\release.
	button.update();

	//True when the button gets pressed in.
	if (button.pressed())
	{
		Serial.println("Button pressed");
	}

	//True when the button gets released.
	if (button.released())
	{
		//getHoldTime will return how long the button was held for. must be called after using button.released() and before next time you call button.update() or else will return 0.
		unsigned int holdTime = button.getHoldTime();
		Serial.print("Released button, held for: ");
		Serial.print(holdTime);
	}

	//Returns the current state of the button.
	currentState = button.current();
}

void onPress()
{
	Serial.println("Pressed button, running onPress function.");
}

void onRelease(unsigned int holdTime)
{
	Serial.print("Released button, held for: ");
	Serial.print(holdTime);
	Serial.println(" running onRelease function.");
}