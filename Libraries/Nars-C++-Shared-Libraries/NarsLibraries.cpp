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

/// <summary>
/// Outputs string from unsigned long input.
/// </summary>
/// <param name="input">: Long number</param>
/// <returns>End string result</returns>
std::string toHex(unsigned long input)
{
	std::stringstream stream;
	stream << std::hex << input;
	std::string output(stream.str());
	std::transform(output.begin(), output.end(), output.begin(), [](unsigned char c) { return std::toupper(c); });
	return output;
}

/// <summary>
/// Outputs string from unsigned long input and string length, replaces empty spots with 0's.
/// </summary>
/// <param name="input">: Long number</param>
/// <param name="stringLength">: End Length</param>
/// <returns>End string result</returns>
std::string toHex(unsigned long input, byte length)
{
	std::string output = toHex(input);
	if (output.length() != length)
	{
		byte originalLength = output.length();
		std::string original = output;
		output = "";
		for (int i = 0; i < length - originalLength; i++)
		{
			output += "0";
		}
		output += original;
	}
	return output;
}

/// <summary>
/// Cast long to ulong with correct signage
/// </summary>
/// <param name="in"></param>
/// <returns>value</returns>
long bitCast(unsigned long in)
{
	return (in - LONG_MAX);
}

/// <summary>
/// Cast long to ulong with correct signage
/// </summary>
/// <param name="in"></param>
/// <returns>value</returns>
unsigned long bitCast(long in)
{
	return (in + LONG_MAX);
}

/// <summary>
/// Map values double
/// </summary>
/// <param name="x">Value to map</param>
/// <param name="in_min">Input min</param>
/// <param name="in_max">Input max</param>
/// <param name="out_min">Output min</param>
/// <param name="out_max">Output max</param>
/// <returns>mapped values</returns>
double mapValue(double x, double in_min, double in_max, double out_min, double out_max) 
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

NarsSerial::NarsSerial()
{
	this->state = NarsSerial::STATES::DISCONNECTED;
	this->userOnRecvHandler = NULL;
	this->first = false;
}

void NarsSerial::setParamerters()
{
	memset(&this->dcb, 0, sizeof(this->dcb));
	this->dcb.DCBlength = sizeof(this->dcb);
	dcb.BaudRate = 1000000;
	dcb.ByteSize = 8;
	dcb.Parity = 0;
	dcb.StopBits = ONESTOPBIT;
	SetCommState(this->port, &this->dcb);
}

RESULT NarsSerial::connect(const char* selectedPort)
{
	RESULT result;
	this->selectedPort = selectedPort;
	if (this->state == STATES::DISCONNECTED)
	{
		if (this->openPort())
		{
			this->setParamerters();
			result.state = RESULT::STATES::CONNECTED;
			result.message = "Connect to port " + (std::string)selectedPort + ".";
			this->state = STATES::CONNECTED;
		}
	}
	else
	{
		result.state = RESULT::STATES::ALREADY_CONNECTED;
		result.message = "Already connected to port.";
	}
	return result;
}

RESULT NarsSerial::check()
{
	RESULT result;
	if (this->state == STATES::CONNECTED)
	{
		this->readLine();
		if ((!this->errorReading) && (strcmp(this->currentMessage, this->oldMessage) != 0))
		{
			strcpy_s(this->oldMessage, sizeof(this->currentMessage), this->currentMessage);
			this->parseData();
			result.state = RESULT::STATES::RECEIVED;
			result.message = std::string(this->currentMessage);
			return result;
		}
		else
		{
			result.state = RESULT::STATES::NONE;
			result.message = "No new message.";
			return result;
		}
	}
	else
	{
		result.state = RESULT::STATES::DISCONNECTED;
		result.message = "Cannot check for data if not conected.";
		return result;
	}
}

RESULT NarsSerial::sendData(unsigned int _register, unsigned long data)
{
	RESULT result;
	if (this->state == NarsSerial::STATES::CONNECTED)
	{
		if (_register <= 65535)
		{
			if (data <= 4294967295)
			{
				std::string completeString = "*D";
				std::string registerString = toHex(_register, 4);
				std::string dataString = toHex(data, 8);
				completeString += registerString + dataString + '-';
				if (writeToPort(completeString))
				{
					result.state = RESULT::STATES::SENT;
					result.message = completeString;
				}
				else
				{
					result.state = RESULT::STATES::LOST;
					result.message = "Unknown rrror sending.";
				}
			}
			else
			{
				result.state = RESULT::STATES::OUT_OF_RANGE;
				result.message = "Data input greater than 4294967295.";
			}
		}
		else
		{
			result.state = RESULT::STATES::OUT_OF_RANGE;
			result.message = "Register input greater than 65535.";
		}
	}
	else
	{
		result.state == RESULT::STATES::DISCONNECTED;
		result.message = "Cannot send data if not connected.";
	}
	return result;
}

RESULT NarsSerial::sendSpecial(unsigned int _register, const char* data)
{
	RESULT result;
	if (this->state == STATES::CONNECTED)
	{
		std::string completeString = "*S";
		if (_register <= 65535)
		{
			std::string registerString = toHex(_register, 4);
			completeString += registerString + data + '-';
			if (writeToPort(completeString))
			{
				result.state = RESULT::STATES::SENT;
				result.message = completeString;
			}
			else
			{
				result.state = RESULT::STATES::LOST;
				result.message = "Unknown rrror sending.";
			}
		}
	}
	return result;
}

void NarsSerial::addOnReceiveHandler(void (*onRecv) (MESSAGE))
{
	this->userOnRecvHandler = onRecv;
}

RESULT NarsSerial::disconnect()
{
	RESULT result;

	return result; 
}

bool NarsSerial::openPort()
{
	std::wstring tws = std::wstring(this->selectedPort.begin(), this->selectedPort.end());
	const wchar_t* sel = tws.c_str();
	this->port = CreateFile(sel , GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	PurgeComm(this->port, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	if (port != INVALID_HANDLE_VALUE)
	{
		return true;
		
	}
	else
	{
		PurgeComm(this->port, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
		return false;
	}
}

void NarsSerial::readLine()
{
	char temp[2];
	byte count = 0;
	DWORD readBytes;
	bool done = false;
	while (!done)
	{
		if (ReadFile(port, temp, 1, &readBytes, NULL))
		{
			if ((readBytes > 0) && (temp[0] != '\n') && (temp[0] != '\r'))
			{
				this->currentMessage[count] = temp[0];
				count++;
			}
			else if (temp[0] == '\r')
			{
				this->currentMessage[count] = '\0';
				done = true;
			}
		}
		else
		{
			printf("Error");
		}
	}
}

int NarsSerial::writeToPort(std::string outputString)
{
	//outputString.insert(0, " ");
	char outputChar[128];
	strcpy_s(outputChar, outputString.c_str());

	int handleResult;
	DWORD bytesSent;
	LPDWORD lpErrors = 0;
	LPCOMSTAT lpstat = 0;
	
	return WriteFile(port, outputChar, outputString.length(), &bytesSent, NULL);
}

void NarsSerial::parseData()
{
	if (sizeof(currentMessage) >= 7)
	{
		MESSAGE message;

		std::string temp = this->currentMessage;
		std::string command = temp.substr(0, 2);

		if (command == "*D")
		{
			if (temp.length() == 15)
			{
				std::string registerString = temp.substr(2, 4);

				char registerBuffer[5];
				strcpy_s(registerBuffer, 5, registerString.c_str());
				message._resgister = x2i((char *)&registerBuffer);

				message.dataType = MESSAGE::DATATYPES::ULONG;
				std::string dataString = temp.substr(6, 8);
				char dataBuffer[9];
				strcpy_s(dataBuffer, 9, dataString.c_str());
				message.data = x2i(dataBuffer);
				message.message = temp;
				message.state = RESULT::STATES::PARSED;

				if (this->userOnRecvHandler != NULL)
				{
					this->userOnRecvHandler(message);
				}
			}
		}

		if (command == "*S")
		{
			std::string command = temp.substr(0, 2);
			std::string registerString = temp.substr(2, 4);

			char registerBuffer[5];
			strcpy_s(registerBuffer, 5, registerString.c_str());
			message._resgister = x2i(registerBuffer);

			message.message = temp;

			message.dataType = MESSAGE::DATATYPES::STRING;
			std::string dataString = "";
			temp.erase(0, 6);

			for (int i = 0; i < temp.length() ; i++)
			{
				char tempChar = (char)temp[i];
				if (tempChar != '-')
				{
					message.special += tempChar;
				}
			}

			message.state = RESULT::STATES::PARSED;

			if (this->userOnRecvHandler != NULL)
			{
				this->userOnRecvHandler(message);
			}
		}
	}
}