/*
 Name:		emptySketch.ino
 Created:	10/24/2021 12:35:35 AM
 Author:	sandr
*/

#include <Wire.h>
#include "NarsObjects.h"

// the setup function runs once when you press reset or power the board
void setup() 
{
	
}

// the loop function runs over and over again until power down or reset
void loop() 
{
  
}

void serialEvent()
{
	SerialCom.onSerialEvent(&done, &special);
}

void done(int _register, unsigned long data)
{

}

void special(int _register, String data)
{

}