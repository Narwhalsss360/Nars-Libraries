#include "NarsSerial.h"

NarsSerial::NarsSerial()
	:queue(), maxQueueSize(DEFAULT_DATA_SIZE), ready(false), dataSize(DEFAULT_DATA_SIZE)
{
	this->pData = new NSDATA[DEFAULT_DATA_SIZE];
}

NarsSerial::NarsSerial(const unsigned short _dataSize)
	:queue(), ready(false), dataSize(_dataSize)
{
	if (_dataSize != NULL)
	{
		this->maxQueueSize = _dataSize;
		this-> pData = new NSDATA[_dataSize];
	}
	else
	{
		this->maxQueueSize = DEFAULT_DATA_SIZE;
		this->pData = new NSDATA[DEFAULT_DATA_SIZE];
	}
}

bool NarsSerial::checkQueue()
{

}

NSDATA NarsSerial::getData(const unsigned short index)
{
	if (index < this->dataSize)
	{

	}
	else
	{
		
	}
}

bool NarsSerial::send(unsigned short index)
{

}

bool NarsSerial::send(unsigned short index, DWORD data)
{

}

NarsSerial::~NarsSerial()
{
	delete[this->dataSize] this->pData;
}