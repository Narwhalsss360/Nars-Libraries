#include "NarsObjects.h"
#include "Wire.h"

/// <summary>
/// Outputs unsinged long from Hex String.
/// </summary>
/// <param name="s">: Hex String</param>
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
/// Outputs string from unsigned long input and string length, replaces empty spots with 0's.
/// </summary>
/// <param name="input">: Long number</param>
/// <param name="stringLength">: End Length</param>
/// <returns>End string result</returns>
String toHex(unsigned long input, byte stringLength) 
{
	String output = String(input, HEX);
	if (output.length() != stringLength)
	{
		byte outLength = output.length();
		String original = output;
		output = "";
		for (int i = 0; i < stringLength - outLength; i++)
		{
			output += "0";
		}
		output += original;
	}
	return output;
}

/// <summary>
/// Returns base-10 from octal input.
/// </summary>
/// <param name="n">: Octal Number</param>
/// <returns>int (Base-10)</returns>
static int octalToDecimal(int n)
{
	int num = n;
	int dec_value = 0;
	int base = 1;
	int temp = num;
	while (temp > 0) {
		int last_digit = temp % 10;
		temp = temp / 10;
		dec_value += last_digit * base;
		base = base * 8;
	}
	return dec_value;
}

/// <summary>
/// Invoke method on serialEvent. After parse, done/special method invokes.
/// </summary>
/// <param name="done">Done function pointer</param>
/// <param name="special">Special function pointer</param>
void NarsSerialCom::onSerialEvent(void (*done) (int _register, unsigned long data), void (*special) (int _register, String data))
{
	String completeString;
	String command;

	while (Serial.available())
	{
		char inChar = (char)Serial.read();
		completeString += inChar;
	}

	command = completeString.substring(0, 2);
	if (command == "*B")
	{
		connected = true;
	}

	if (command == "*D")
	{
		String registerString = completeString.substring(2, 6);;
		String dataString = completeString.substring(6, 14);;
		int _register;
		unsigned long _data;
		char _registerBuffer[4];
		char dataBuffer[8];

		strcpy(_registerBuffer, registerString.c_str());
		strcpy(dataBuffer, dataString.c_str());

		_register = x2i(_registerBuffer);
		_data = x2i(dataBuffer);

		if (_register <= 512)
		{
			data[_register] = _data;
		}
		done(_register, _data);
	}

	if (command == "*S")
	{
		String registerString = completeString.substring(2, 6);
		int _register;

		char _registerBuffer[4];
		strcpy(_registerBuffer, registerString.c_str());

		String specialString;
		String temp = completeString;
		temp.remove(0, 6);
		for (int i = 0; i < temp.length() - 1; i++)
		{
			char tempChar = (char)temp[i];
			if (tempChar != '-')
			{
				specialString += tempChar;
			}
		}
		special(_register, specialString);
	}

	if (command == "*E")
	{
		connected = false;
	}
}			

/// <summary>
/// Invoke method on serialEvent. After parse, done/special method invokes.
/// </summary>
/// <param name="special">Special function pointer</param>
void NarsSerialCom::onSerialEvent(void (*special) (int _register, String data))
{
	String completeString;
	String command;

	while (Serial.available())
	{
		char inChar = (char)Serial.read();
		completeString += inChar;
	}

	command = completeString.substring(0, 2);
	if (command == "*B")
	{
		connected = true;
	}

	if (command == "*D")
	{
		String registerString = completeString.substring(2, 6);;
		String dataString = completeString.substring(6, 14);;
		int _register;
		unsigned long _data;
		char _registerBuffer[4];
		char dataBuffer[8];

		strcpy(_registerBuffer, registerString.c_str());
		strcpy(dataBuffer, dataString.c_str());

		_register = x2i(_registerBuffer);
		_data = x2i(dataBuffer);

		if (_register <= 512)
		{
			data[_register] = _data;
		}
	}

	if (command == "*S")
	{
		String registerString = completeString.substring(2, 6);
		int _register;

		char _registerBuffer[4];
		strcpy(_registerBuffer, registerString.c_str());

		String specialString;
		String temp = completeString;
		temp.remove(0, 6);
		for (int i = 0; i < temp.length() - 1; i++)
		{
			char tempChar = (char)temp[i];
			if (tempChar != '-')
			{
				specialString += tempChar;
			}
		}
		special(_register, specialString);
	}

	if (command == "*E")
	{
		connected = false;
	}
}

/// <summary>
/// Check if client is available.
/// </summary>
void WireHost::check()
{
	Wire.beginTransmission(this->deviceProperties.address);
	Wire.write(0x00);
	Wire.endTransmission();
	if (Wire.available())
	{
		notFoundCounter = 0;
		this->deviceProperties.id = Wire.read();
		this->deviceProperties.state = this->deviceProperties.states.CONNECTED;
	}
	else
	{
		if (notFoundCounter == 255)
		{
			this->deviceProperties.state = this->deviceProperties.states.DISCONNECTED;
		}
		else
		{
			notFoundCounter++;
		}
	}
}

/// <summary>
/// Get data from all registers.
/// </summary>
void WireHost::getData()
{
	if (this->deviceProperties.state == this->deviceProperties.states.CONNECTED)
	{
		for (byte _register = 1; _register < 64; _register++)
		{
			Wire.beginTransmission(this->deviceProperties.address);
			Wire.write(_register);
			Wire.endTransmission();
			Wire.requestFrom(this->deviceProperties.address, (byte)1);
			if (1 <= Wire.available())
			{
				this->deviceProperties.data[_register] = Wire.read();
			}
		}
	}
}

/// <summary>
/// Send data from all registers.
/// </summary>
void WireHost::sendData()
{
	if (this->deviceProperties.state == this->deviceProperties.states.CONNECTED)
	{
		for (byte _register = 65; _register < 255; _register++)
		{
			Wire.beginTransmission(this->deviceProperties.address);
			Wire.write(_register);
			Wire.endTransmission();
			Wire.beginTransmission(this->deviceProperties.address);
			Wire.write(this->deviceProperties.data[_register]);
			Wire.endTransmission();
		}
	}
}

/// <summary>
/// Send data from specific register.
/// </summary>
/// <param name="addr">Specific register</param>
void WireHost::sendData(byte addr)
{
	if (this->deviceProperties.state == this->deviceProperties.states.CONNECTED)
	{
		Wire.beginTransmission(this->deviceProperties.address);
		Wire.write(addr);
		Wire.endTransmission();
		Wire.beginTransmission(this->deviceProperties.address);
		Wire.write(this->deviceProperties.data[addr]);
		Wire.endTransmission();
	}
}

/// <summary>
/// Invoke in Wire.OnReceive
/// </summary>
/// <param name="bytes">On Receive</param>
void WireClient::onReceive(int bytes)
{
	this->recvData = Wire.read();
	if (this->recvData >= 65)
	{
		if (this->recv = false)
		{
			this->recv = true;
		}
		this->registerSelect = this->recvData;
	}
	if (recv)
	{
		this->deviceProperties.data[this->registerSelect] = recvData;
	}
	else
	{
		this->registerSelect = this->recvData;
	}
}

/// <summary>
/// Invoke on Wire.onRequest
/// </summary>
void WireClient::onRequest()
{
	if (!recv)
	{
		Wire.write(this->deviceProperties.data[this->registerSelect]);
	}
}