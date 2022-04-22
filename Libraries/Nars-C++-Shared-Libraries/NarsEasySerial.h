#pragma once

#include <Windows.h>
#include "NarsUsefuls.h"
#include <stdio.h>

#define NarsEasySerial_h
#define DEFAULT_BAUD 9600

enum class NESRETURNS
{
	SUCCESS,
	E_INVALID_HANDLE_VALUE,
	E_GETCOMMSTATE,
	E_SETCOMMSTATE,
	W_NOTCONNECTED,
	E_WRITE,
	E_READ,
	E_READLINE,
	E_SMALLBUFFER,
	W_NONL
};

struct _IORET
{
	DWORD lastError;
	DWORD h_IO;
	LPDWORD h_Errors;
	LPCOMSTAT h_ComStat;
};

typedef _IORET IORET, *pIORET;

const char* getReturnString(NESRETURNS);

class NarsEasySerial
{
public:
	NarsEasySerial();
	~NarsEasySerial();
	NESRETURNS getLastError();
	NESRETURNS connect();
	NESRETURNS write(LPCVOID, DWORD, pIORET);
	NESRETURNS read(LPVOID, DWORD, pIORET);
	NESRETURNS readLine(char*, DWORD, pIORET);
	NESRETURNS readLineLossy(char*, DWORD, pIORET);
	NESRETURNS disconnect();
	NESRETURNS disconnect(BOOL*);
	void selectPort(DWORD);
	void setBaudRate(DWORD);
	bool status();
	HANDLE* getHandle();
private:
	void convertPort();
	void purge();
	HANDLE h_Port;
	DCB dcb;
	DWORD selectedPort;
	char port[10];
	NESRETURNS lastError;
	DWORD bRate;
	bool connected;
};