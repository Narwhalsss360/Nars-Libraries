#include <NarsLibraries.h>

unsigned long myDataToSend1 = 321342;
unsigned long myDataToSend2 = 3364;
unsigned long myDataToSend3 = 76683;

unsigned long myDataRecieved1;
unsigned long myDataRecieved2;

String mySpecialToSend3 = "DATA!";
String mySpecialToReceive4;

void setup() 
{
	Serial.begin(1000000);
	SerialCom.connected = true;
}

//Pretend interrupt if you use an attatched interrupt
void pretendInterrupt()
{
	SerialCom.unsetReady();
}

void loop() 
{
	//send data at a register, then the data itself
	SerialCom.send(1, myDataToSend1);

	SerialCom.data[2] = myDataToSend2;
	SerialCom.data[3] = myDataToSend3;

	//send data from many ranges from SerialCom.data array
	SerialCom.sendDataRange(2, 3);

	SerialCom.sendSpecial(3, mySpecialToSend3);

	//must set ready at end of code so that you know when arduino is available
	SerialCom.setReady();
}

//Arduino's on serial event
void serialEvent()
{
	//Add to serialEvent, then add onReceive handler, then add onRecieve of special data (String)
	SerialCom.onSerialEvent(onRecieve, onSpecialRecieve);

	myDataRecieved2 = SerialCom.data[2];

	//If you dont want your own on recv handlers use:
	//SerialCom.onSerialEvent(null, null);
}

//Your own recv event
void onRecieve(unsigned int _register, unsigned long data)
{
	//Gets received register, and data
	if (_register == 1)
	{
		myDataRecieved1 = data;
	}
}

//Your own recv event
void onSpecialRecieve(unsigned int _register, String data)
{
	//Gets received register, and data
	if (_register == 3)
	{
		mySpecialToReceive4 = data;
	}
}