#include "NarsLibraries.h"

bool NarsSerialCom::connected;
unsigned long NarsSerialCom::data[256];
bool NarsSerialCom::ready;

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
/// Outputs string from unsigned long input and string length, replaces empty spots with 0's.
/// </summary>
/// <param name="input">: Long number</param>
/// <param name="stringLength">: End Length</param>
/// <returns>End string result</returns>
String toHex(unsigned long input, byte stringLength) 
{
	String toggled = String(input, HEX);
	toggled.toUpperCase();
	if (toggled.length() != stringLength)
	{
		byte outLength = toggled.length();
		String original = toggled;
		toggled = "";
		for (int i = 0; i < stringLength - outLength; i++)
		{
			toggled += "0";
		}
		toggled += original;
	}
	return toggled;
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
/// Cast ulong to long with correct signage
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

/// <summary>
/// The Horner Scheme
/// </summary>
/// <param name="Num"></param>
/// <param name="Divider"></param>
/// <param name="Factor"></param>
/// <returns></returns>
unsigned long hornerScheme(unsigned long Num, unsigned long Divider, unsigned long Factor)
{
	unsigned long Remainder = 0, Quotient = 0, Result = 0;
	Remainder = Num % Divider;
	Quotient = Num / Divider;
	if (!(Quotient == 0 && Remainder == 0))
		Result += hornerScheme(Quotient, Divider, Factor) * Factor + Remainder;
	return Result;
}

/// <summary>
/// 4-Digit to xxx.xxx
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
double intToFreq(double input)
{
	input += 10000;
	input /= 100;
	return input;
}

/// <summary>
/// Converts bool to string "True"/"False"
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
String boolToString(bool input)
{
	if (input)
	{
		return "True";
	}
	else
	{
		return "False";
	}
}

void byteWrite(byte pin, byte byteOut) {
	for (byte i = 0; i < 8; i++) {
		digitalWrite(pin, LOW);
		if (0x80 & byteOut) digitalWrite(pin, HIGH);
		else digitalWrite(pin, LOW);
		digitalWrite(pin, HIGH);
		digitalWrite(pin, LOW);
		byteOut <<= 1;
	}
}

/// <summary>
/// Invoke method on serialEvent. After parse, done/special method invokes.
/// </summary>
/// <param name="done">Done function pointer</param>
/// <param name="special">Special function pointer</param>
void NarsSerialCom::onSerialEvent(void (*done) (unsigned int _register, unsigned long data), void (*special) (unsigned int _register, String data))
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
		unsetReady();
		String registerString = completeString.substring(2, 6);;
		String dataString = completeString.substring(6, 14);;
		unsigned int _register;
		unsigned long _data;
		char _registerBuffer[4];
		char dataBuffer[8];

		strcpy(_registerBuffer, registerString.c_str());
		strcpy(dataBuffer, dataString.c_str());

		_register = x2i(_registerBuffer);
		_data = x2i(dataBuffer);

		if (_register <= 256)
		{
			data[_register] = _data;
		}
		done(_register, _data);
	}

	if (command == "*S")
	{
		unsetReady();
		String registerString = completeString.substring(2, 6);
		unsigned int _register;
		char _registerBuffer[4];
		strcpy(_registerBuffer, registerString.c_str());
		_register = x2i(_registerBuffer);

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
		specialString.remove(specialString.length() - 1);
		special(_register, specialString);
	}

	if (command == "*E")
	{
		connected = false;
	}

}			

/// <summary>
/// Sends data
/// </summary>
/// <param name="_register">register</param>
/// <param name="data">data</param>
void NarsSerialCom::send(unsigned int _register, unsigned long data)
{
	if (connected)
	{
		String completeString = "*D";
		if (_register <= 65535 || data <= 4294967295)
		{
			String registerString = toHex(_register, 4);
			String dataString = toHex(data, 8);
			completeString += registerString + dataString + '-';
			Serial.println(completeString);
		}
	}
}

/// <summary>
/// Sends special data
/// </summary>
/// <param name="_register">register</param>
/// <param name="data">data</param>
void NarsSerialCom::sendSpecial(unsigned int _register, String data)
{
	if (connected)
	{
		String completeString = "*S";
		if (_register <= 65535)
		{
			String registerString = toHex(_register, 4);
			completeString += registerString + data + '-';
			Serial.println(completeString);
		}
	}
}

/// <summary>
/// Send range of data
/// </summary>
/// <param name="start"></param>
/// <param name="end"></param>
void NarsSerialCom::sendDataRange(uint8_t start, uint8_t end)
{
	for (uint8_t i = start; i <= end; i++)
	{
		send(i, data[i]);
	}
}

/// <summary>
/// Set Ready
/// </summary>
void NarsSerialCom::setReady()
{
	ready = true;
	send(0, 1);
}

/// <summary>
/// Unset Ready
/// </summary>
void NarsSerialCom::unsetReady()
{
	ready = false;
	send(0, 0);
}

/// <summary>
/// Push Toggle class
/// </summary>
/// <param name="_pin"></param>
/// <param name="_invert"></param>
/// <param name="_debounceDelay"></param>
PushToggle::PushToggle(byte _pin, bool _invert, byte _debounceDelay)
{
	this->pin = _pin;
	this->invertedInput = _invert;
	this->debounceDelay = _debounceDelay;
}

/// <summary>
/// Check button with debounce and toggle
/// </summary>
void PushToggle::read()
{
	bool pinState;

	if (this->invertedInput)
	{
		pinState = !digitalRead(this->pin);
	}
	else
	{
		pinState = digitalRead(this->pin);
	}
	
	if (pinState!= this->lastButtonState)
	{
		this->lastDebounceTime = millis();
	}

	if ((millis() - this->lastDebounceTime) > this->debounceDelay)
	{
		if (pinState != this->endState)
		{
			this->endState = pinState;

			if (endState)
			{
				this->toggle = !this->toggle;
			}
		}
	}

	this->lastButtonState = pinState;
}

/// <summary>
/// Check of toggled
/// </summary>
/// <returns></returns>
bool PushToggle::toggled()
{
	return this->toggle;
}

#ifdef TwoWire_h
/// <summary>
/// Search for wire devices.
/// </summary>
/// <returns>Device Addresses</returns>
String wireSearch()
{
	String addresses;
	for (byte i = 1; i < 127; i++)
	{
		Wire.beginTransmission(i);
		byte status = Wire.endTransmission();
		if (status == 0)
		{
			addresses += (String)i;
			addresses += ",";
		}
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
#endif // TwoWire_h