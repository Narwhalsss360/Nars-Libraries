#pragma once
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

unsigned long x2i(char* s);

std::string toHex(unsigned long input);

std::string toHex(unsigned long input, byte length);

long bitCast(unsigned long in);

unsigned long bitCast(long in);

double mapValue(double x, double in_min, double in_max, double out_min, double out_max);

struct RESULT
{
public:
	static enum class STATES
	{
		NONE,
		CONNECTED,
		ALREADY_CONNECTED,
		DISCONNECTED,
		ALREADY_DISCONNECTED,
		OUT_OF_RANGE,
		LOST,
		SENT,
		RECEIVED,
		PARSED,
		COM_ERROR
	};

	STATES state = STATES::NONE;
	std::string message;
};

struct MESSAGE : public RESULT
{
public:
	static enum DATATYPES
	{
		STRING,
		ULONG
	};

	DATATYPES dataType;
	DWORD _resgister;
	DWORD data;
	std::string special;
};

class NarsSerial
{
public:
	NarsSerial();

	enum class STATES
	{
		DISCONNECTED,
		CONNECTED
	};

	STATES state;
	std::string selectedPort;

	RESULT connect(const char* selectedPort);
	RESULT check();
	RESULT sendData(unsigned int _register, unsigned long data);
	RESULT sendSpecial(unsigned int _register, const char* data);
	void addOnReceiveHandler(void (*onRecv) (MESSAGE));
	RESULT disconnect();
private:
	HANDLE port;
	DCB dcb;

	bool errorReading;
	void (*userOnRecvHandler)(MESSAGE);
	unsigned int currentRegister;
	char oldMessage[256];
	char currentMessage[256];
	void setParamerters();
	bool openPort();
	void readLine();
	void parseData();
	int writeToPort(std::string outputString);
	bool first;
};
