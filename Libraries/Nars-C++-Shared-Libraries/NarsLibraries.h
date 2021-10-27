#pragma once
#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <string>

unsigned long x2i(char* s);

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
		PARSED
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
	DWORD data[65535];
	std::string selectedPort;

	RESULT connect(const char* selectedPort);
	RESULT check();
	MESSAGE getData();
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
	bool first;
};
