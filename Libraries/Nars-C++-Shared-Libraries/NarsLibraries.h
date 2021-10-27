#pragma once
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <string>

unsigned long x2i(char* s);

struct RESULT
{
public:
	static enum STATES
	{
		CONNECTED,
		ALREADY_CONNECTED,
		DISCONNECTED,
		ALREADY_DISCONNECTED,
		OUT_OF_RANGE,
		LOST,
		SENT,
		RECEIVED
	};

	STATES returnState;
	std::string message;
};

struct MESSAGE : public RESULT
{
public:
	static enum dataTypes
	{
		STRING,
		ULONG
	};

	byte dataType;
	DWORD _resgister;
	DWORD data;
};

class NarsSerial
{
public:
	NarsSerial();
	enum STATES
	{

	};

	byte state;
	std::string data[65535];

	RESULT connect(const char* selectedPort);
	MESSAGE getData();
	RESULT sendData(unsigned int _register, unsigned long data);
	RESULT sendSpecial(unsigned int _register, const char* data);
	RESULT check();
	void addOnReceiveHandler(void (*onRecv) (MESSAGE));
private:
	unsigned int currentRegister;
};
