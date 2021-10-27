#include "NarsLibraries.h"

/// <summary>
/// Outputs unsinged long from Hex Char array.
/// </summary>
/// <param name="s">: Char Array</param>
/// <returns>Number From Hex</returns>
unsigned long x2i(char* s)
{
	unsigned long x = 0;
	for (;;)
	{
		char c = *s;
		if (c >= '0' && c <= '9')
		{
			x *= 16;
			x += c - '0';
		}
		else if (c >= 'A' && c <= 'F')
		{
			x *= 16;
			x += (c - 'A') + 10;
		}
		else break;
		s++;
	}
	return x;
}

NarsSerial::NarsSerial()
{
	this->state = this->DISCONNECTED;
	this->userOnRecvHandler = NULL;
	this->setParamerters();
}

void NarsSerial::setParamerters()
{
	memset(&this->dcb, 0, sizeof(this->dcb));
	this->dcb.DCBlength = sizeof(this->dcb);
}

RESULT NarsSerial::connect(const char* selectedPort)
{
	RESULT result;

	return result;
}

RESULT NarsSerial::check()
{
	RESULT result;
	std::string temp;

	return result;
}

MESSAGE NarsSerial::getData()
{
	MESSAGE message;

	return message;
}

RESULT NarsSerial::sendData(unsigned int _register, unsigned long data)
{
	RESULT result;

	return result;
}

RESULT NarsSerial::sendSpecial(unsigned int _register, const char* data)
{
	RESULT result;

	return result;
}

void NarsSerial::addOnReceiveHandler(void (*onRecv) (MESSAGE))
{
	this->userOnRecvHandler = onRecv;
}

void NarsSerial::onNewData(MESSAGE message)
{
	if (this->userOnRecvHandler != NULL)
	{
		this->userOnRecvHandler(message);
	}
}

RESULT NarsSerial::disconnect()
{
	RESULT result;

	return result; 
}