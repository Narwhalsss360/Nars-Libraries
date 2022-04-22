#pragma once

#include <Windows.h>
#include "NarsUsefuls.h"

#define NarsArduinoSerial_h

enum class NASRETURNS
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

const char* getReturnString(NASRETURNS);

class NarsArduinoSerial
{
public:
	NarsArduinoSerial();
	~NarsArduinoSerial();
	NASRETURNS getLastError();
	NASRETURNS connect();
	NASRETURNS write(LPCVOID, DWORD, pIORET);
	NASRETURNS read(LPCVOID, DWORD, pIORET);
	NASRETURNS readLine(char*, DWORD, pIORET);
	NASRETURNS readLineLossy(char*, DWORD, pIORET);
	NASRETURNS disconnect();
	NASRETURNS disconnect(BOOL*);
	void selectPort(DWORD);
	void setBaudRate(DWORD);
private:
	HANDLE h_Port;
	DCB dcb;
	DWORD selectedPort;
	NASRETURNS lastError;
	DWORD buadRate;
};