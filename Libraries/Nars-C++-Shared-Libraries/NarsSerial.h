#pragma once
#include "NarsEasySerial.h"
#include <queue>

#define NarsSerial_h
#define DEFAULT_DATA_SIZE 65535
#define MAX_DATA_SIZE 65535

enum class NSRETURNS
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
	W_NONL,
	INDEX_OUT_OF_RANGE
};

struct _NSDATA
{
	DWORD DATA;
	bool u;
};

typedef _NSDATA NSDATA, *PNSDATA;

class NarsSerial : NarsEasySerial
{
public:
	NarsSerial();
	NarsSerial(const unsigned short);
	~NarsSerial();
	bool checkQueue();
	NSDATA getData(const unsigned short);
	bool send(unsigned short);
	bool send(unsigned short, DWORD);
	PNSDATA pData;
private:
	std::queue<NSDATA> queue;
	DWORD maxQueueSize;
	const DWORD dataSize;
	bool ready;
};