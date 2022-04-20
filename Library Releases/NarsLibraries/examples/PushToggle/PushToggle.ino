/*
	This class is used to use a push button as a toggle with debounce

	Use INVERTED as true if using INPUT_PULLUP, false if not
*/

#include <NarsLibraries.h>

#define BUTTON_PIN 4
#define LED LED_BUILTIN
#define INVERTED true
#define DEBOUNCE_TIME 50

//					  INPUT_PIN, INVERTED or NOT, DEBOUNCE TIME in ms
PushToggle pushToggle1(BUTTON_PIN, INVERTED, DEBOUNCE_TIME);

//object to save whether toggled ON or OFF (REDUNDANT)
bool toggleState;

void setup() 
{
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
}

void loop() 
{
	//Read button and toggle respectivly
	pushToggle1.read();

	//save if toggled or not, you could just use 'pushToggle1.toggled();'
	toggleState = pushToggle1.toggled();

	//LED
	digitalWrite(LED, toggleState);
}
