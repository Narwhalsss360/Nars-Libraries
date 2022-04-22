#pragma once
#include "NarsEasySerial.h"
#include <queue>

#define NarsSerial_h
#define DEFAULT_DATA_SIZE 65535
#define MAX_DATA_SIZE 65535

enum class NSRETURNS
{
	
}

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
	NSDATA getData(const unsigned short);
	bool send(unsigned short);
	bool send(unsigned short, DWORD);
	PNSDATA pData;
private:
	std::queue<NSDATA> queue;
	DWORD maxQueueSize;
	bool ready;
};