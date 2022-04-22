#include "NarsArduinoSerial.h"

NarsArduinoSerial::NarsArduinoSerial()
{
}

NarsArduinoSerial::~NarsArduinoSerial()
{
	CloseHandle(this->h_Port);
}