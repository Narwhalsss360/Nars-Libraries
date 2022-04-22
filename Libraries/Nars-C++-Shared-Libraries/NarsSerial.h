#pragma once
#include "NarsEasySerial.h"
#include <queue>

struct _NSDATA
{
	DWORD DATA;
	bool u;
};

typedef _NSDATA NSDATA, *PNSDATA;

class NarsSerial : NarsEasySerial
{
public:
	NarsSerial(DWORD);
	~NarsSerial();
	bool checkQueue();
	NSDATA getData(unsigned short);
	bool send(unsigned short);
	bool send(unsigned short, DWORD);
	PNSDATA pData;
private:
	std::queue<DWORD> queue;
	bool ready;
};

#define NarsSerial_h