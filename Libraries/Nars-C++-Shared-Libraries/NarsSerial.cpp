#include "NarsSerial.h"

NarsSerial::NarsSerial()
	:queue(), maxQueueSize(DEFAULT_DATA_SIZE), ready(false)
{
	this->pData = new NSDATA[DEFAULT_DATA_SIZE];
}

NarsSerial::NarsSerial(const unsigned short dataSize)
	: queue(), ready(false)
{
	if (dataSize != NULL)
	{
		this->maxQueueSize = dataSize;
		this-> pData = new NSDATA[dataSize];
	}
	else
	{
		this->maxQueueSize = DEFAULT_DATA_SIZE;
		this->pData = new NSDATA[DEFAULT_DATA_SIZE];
	}
}