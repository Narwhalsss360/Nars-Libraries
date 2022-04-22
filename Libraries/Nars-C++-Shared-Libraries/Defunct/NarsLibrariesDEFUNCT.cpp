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
	std::string toggled(stream.str());
	std::transform(toggled.begin(), toggled.end(), toggled.begin(), [](unsigned char c) { return std::toupper(c); });
	return toggled;
}

/// <summary>
/// Outputs string from unsigned long input and string length, replaces empty spots with 0's.
/// </summary>
/// <param name="input">: Long number</param>
/// <param name="stringLength">: End Length</param>
/// <returns>End string result</returns>
std::string toHex(unsigned long input, byte length)
{
	std::string toggled = toHex(input);
	if (toggled.length() != length)
	{
		byte originalLength = toggled.length();
		std::string original = toggled;
		toggled = "";
		for (int i = 0; i < length - originalLength; i++)
		{
			toggled += "0";
		}
		toggled += original;
	}
	return toggled;
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
		else
		{
			std::ostringstream stream;
			stream << GetLastError();
			result.message = stream.str();
			result.state = RESULT::STATES::COM_ERROR;
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
				if (this->ready)
				{
					if (writeToPort(completeString))
					{
						this->lastResult.state = RESULT::STATES::SENT;
						this->lastResult.message = completeString;
						this->ready = false;
						return this->lastResult;
					}
					else
					{
						this->lastResult.state = RESULT::STATES::LOST;
						this->lastResult.message = "Unkown error sending";
						return this->lastResult;
					}
				}
				else
				{
					this->sendQueue.push(completeString); 
					this->lastResult.state = RESULT::STATES::QUEUED;
					this->lastResult.message = "Queued";
					return this->lastResult;
				}
			}
			else
			{
				this->lastResult.state = RESULT::STATES::OUT_OF_RANGE;
				this->lastResult.message = "Data out of range";
				return this->lastResult;
			}
		}
		else
		{
			this->lastResult.state = RESULT::STATES::OUT_OF_RANGE;
			this->lastResult.message = "Register out of range";
			return this->lastResult;
		}
	}
	else
	{
		this->lastResult.state = RESULT::STATES::DISCONNECTED;
		this->lastResult.message = "Not connected";
		return this->lastResult;
	}
}

RESULT NarsSerial::sendSpecial(unsigned int _register, const char* data)
{
	if (this->state == NarsSerial::STATES::CONNECTED)
	{
		if (this->ready)
		{
			if (_register <= 65535)
			{
				std::string completeString = "*D";
				std::string registerString = toHex(_register, 4);
				completeString += registerString + data + '-';
				if (writeToPort(completeString))
				{
					this->lastResult.state = RESULT::STATES::SENT;
					this->lastResult.message = completeString;
					return this->lastResult;
					this->ready = false;
				}
				else
				{
					this->lastResult.state = RESULT::STATES::LOST;
					this->lastResult.message = "Unkown error sending";
					return this->lastResult;
				}
			}
			else
			{
				this->lastResult.state = RESULT::STATES::OUT_OF_RANGE;
				this->lastResult.message = "Register out of range";
				return this->lastResult;
			}
		}
		else
		{
			this->lastResult.state = RESULT::STATES::NOT_READY;
			this->lastResult.message = "Not ready";
			return this->lastResult;
		}
	}
	else
	{
		this->lastResult.state = RESULT::STATES::DISCONNECTED;
		this->lastResult.message = "Not connected";
		return this->lastResult;
	}
}

RESULT NarsSerial::checkQueue()
{
	if (this->state == NarsSerial::STATES::CONNECTED)
	{
		if (this->sendQueue.size() > 0)
		{
			if (this->ready)
			{
				std::string message = this->sendQueue.front();
				this->sendQueue.pop();
				if (writeToPort(message))
				{
					this->lastResult.state = RESULT::STATES::SENT;
					this->lastResult.message = message;
					return this->lastResult;
					this->ready = false;
				}
				else
				{
					this->lastResult.state = RESULT::STATES::LOST;
					this->lastResult.message = "Unkown error sending";
					return this->lastResult;
				}
			}
			else
			{
				this->lastResult.state = RESULT::STATES::NOT_READY;
				this->lastResult.message = "Not ready";
				return this->lastResult;
			}
		}
		else
		{
			this->lastResult.state = RESULT::STATES::EMPTY;
			this->lastResult.message = "Empty Queue";
			return this->lastResult;
		}
	}
	else
	{
		this->lastResult.state = RESULT::STATES::DISCONNECTED;
		this->lastResult.message = "Not Connected";
		return this->lastResult;
	}
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
	this->port = CreateFile((LPCWSTR)sel, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
		
		int result = ReadFile(this->port, temp, 1, &readBytes, NULL);

		if (result)
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
			done = true;
		}
	}
}

int NarsSerial::writeToPort(std::string outputString)
{
	//outputString.insert(0, " ");
	char outputChar[128];
	//strcpy_s(outputChar, outputString.c_str());

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

				if (message._resgister == 0)
				{
					if (message.data)
					{
						this->ready = true;
					}
					else
					{
						this->ready = false;
					}
				}

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

bool NarsSerial::getReady()
{
	return this->ready;
}