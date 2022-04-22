#include "NarsEasySerial.h"

NarsEasySerial::NarsEasySerial()
	:h_Port(NULL), dcb(), selectedPort(ZERO), lastError(NESRETURNS::SUCCESS), bRate(DEFAULT_BAUD), port(), connected(false)
{
}

void NarsEasySerial::convertPort()
{
	sprintf_s(this->port, "\\\\.\\COM%d", this->selectedPort);
}

void NarsEasySerial::purge()
{
	PurgeComm(this->h_Port, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
}

HANDLE* NarsEasySerial::getHandle()
{
	return &this->h_Port;
}

NESRETURNS NarsEasySerial::getLastError()
{
	return this->lastError;
}

NESRETURNS NarsEasySerial::connect()
{
	this->convertPort();
	this->h_Port = CreateFileA((LPCSTR)this->port, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	this->purge();

	if (this->h_Port == INVALID_HANDLE_VALUE)
	{
		this->disconnect();
		return this->lastError = NESRETURNS::E_INVALID_HANDLE_VALUE;
	}

	SecureZeroMemory(&this->dcb, sizeof(DCB));

	if (!GetCommState(this->h_Port, &this->dcb))
	{
		CloseHandle(this->h_Port);
		return this->lastError = NESRETURNS::E_GETCOMMSTATE;
	}

	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = this->bRate;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;
	dcb.ByteSize = 8;

	if (!SetCommState(this->h_Port, &this->dcb))
	{
		CloseHandle(this->h_Port);
		return this->lastError = NESRETURNS::E_SETCOMMSTATE;
	}
	this->connected = true;
	return this->lastError = NESRETURNS::SUCCESS;
}

void NarsEasySerial::selectPort(DWORD _port)
{
	this->selectedPort = _port;
}

void NarsEasySerial::setBaudRate(DWORD _bRate)
{
	this->bRate = _bRate;
}

NESRETURNS NarsEasySerial::write(LPCVOID buf, DWORD szBuf, pIORET ret)
{
	int status = ZERO;
	DWORD transmitted = ZERO;
	LPDWORD errors = NULL;
	LPCOMSTAT comStat = NULL;

	status = WriteFile(this->h_Port, buf, szBuf, &transmitted, NULL);

	if (ret != NULL)
	{
		*ret = { GetLastError(), transmitted, errors, comStat };
	}

	if (!status)
	{
		ClearCommError(this->h_Port, errors, comStat);
		this->connected = false;
		this->disconnect();
		return this->lastError = NESRETURNS::E_WRITE;
	}
	else
	{
		return this->lastError = NESRETURNS::SUCCESS;
	}
}

NESRETURNS NarsEasySerial::read(LPVOID buf, DWORD szBuf, pIORET ret)
{
	int status = ZERO;
	DWORD read = ZERO;
	LPDWORD errors = NULL;
	LPCOMSTAT comStat = NULL;

	status = ReadFile(this->h_Port, buf, szBuf, &read, NULL);

	if (ret != NULL)
	{
		*ret = { GetLastError(), read, errors, comStat };
	}

	if (!status)
	{
		ClearCommError(h_Port, errors, comStat);
		this->connected = false;
		this->disconnect();
		return this->lastError = NESRETURNS::E_READ;
	}
	else
	{
		return this->lastError = NESRETURNS::SUCCESS;
	}
}

NESRETURNS NarsEasySerial::readLine(char* buf, DWORD szBuf, pIORET ret)
{
	DWORD count = 0;
	char tempBuf[2];
	NESRETURNS status;

	while (true)
	{
		status = read(tempBuf, 2, ret);

		if (status != NESRETURNS::SUCCESS)
		{
			return this->lastError = NESRETURNS::E_READLINE;
		}

		if ((ret->h_IO > 0) && (tempBuf[0] != '\n') && (tempBuf[0] != '\r'))
		{
			*buf = tempBuf[0];
			if (count < szBuf)
			{
				count++;
				buf++;
			}
			else
			{
				return this->lastError = NESRETURNS::E_SMALLBUFFER;
			}
		}
		else if ((tempBuf[0] == '\n') || (tempBuf[0] == '\r'))
		{
			*buf = '\0';
			return this->lastError = NESRETURNS::SUCCESS;
		}
	}
}

NESRETURNS NarsEasySerial::readLineLossy(char* buf, DWORD szBuf, pIORET ret)
{
	NESRETURNS status = read(buf, szBuf, ret);
	DWORD count = 0;

	if (status != NESRETURNS::SUCCESS)
	{
		return status;
	}

	while (true)
	{
		if (*buf != '\n')
		{
			if (count < szBuf)
			{
				count++;
				buf++;
			}
			else
			{
				return this->lastError = NESRETURNS::E_SMALLBUFFER;
			}
		}
		else
		{
			for (DWORD i = 0; i < (szBuf - count); i++)
			{
				*buf = ' ';
				buf++;
			}
			return this->lastError = NESRETURNS::SUCCESS;
		}
	}
}

NESRETURNS NarsEasySerial::disconnect()
{
	if (this->h_Port != INVALID_HANDLE_VALUE && this->h_Port != NULL)
	{
		CloseHandle(this->h_Port);
		this->h_Port = NULL;
		this->connected = false;
		return this->lastError = NESRETURNS::SUCCESS;
	}
	else
	{
		return this->lastError = NESRETURNS::W_NOTCONNECTED;
	}
}

NESRETURNS NarsEasySerial::disconnect(BOOL* closeRet)
{
	if (this->h_Port != INVALID_HANDLE_VALUE && this->h_Port != NULL)
	{
		*closeRet = CloseHandle(this->h_Port);
		this->h_Port = NULL;
		this->connected = false;
		return this->lastError = NESRETURNS::SUCCESS;
	}
	else
	{
		return this->lastError = NESRETURNS::W_NOTCONNECTED;
	}
}

bool NarsEasySerial::status()
{
	return this->connected;
}

NarsEasySerial::~NarsEasySerial()
{
	if (this->h_Port != NULL)
	{
		CloseHandle(this->h_Port);
	}
}