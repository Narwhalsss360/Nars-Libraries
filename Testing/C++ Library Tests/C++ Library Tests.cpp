// C++ Library Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <sstream>
#include <iostream>
#include <algorithm>
#include "NarsLibraries.h"

NarsSerial ns = NarsSerial();

std::string getResultString(RESULT::STATES state)
{
	switch (state)
	{
	case RESULT::STATES::NONE:
		return "NONE";
		break;
	case RESULT::STATES::CONNECTED:
		return "CONNECTED";
		break;
	case RESULT::STATES::ALREADY_CONNECTED:
		return "ALREADY_CONNECTED";
		break;
	case RESULT::STATES::DISCONNECTED:
		return "DISCONNECTED";
		break;
	case RESULT::STATES::ALREADY_DISCONNECTED:
		return "ALREADY_DISCONNECTED";
		break;
	case RESULT::STATES::OUT_OF_RANGE:
		return "OUT_OF_RANGE";
		break;
	case RESULT::STATES::LOST:
		return "LOST";
		break;
	case RESULT::STATES::SENT:
		return "SENT";
		break;
	case RESULT::STATES::RECEIVED:
		return "RECEIVED";
		break;
	case RESULT::STATES::PARSED:
		return "PARSED";
		break;
	default:
		return "n/a";
		break;
	}
}

void onRecv(MESSAGE message)
{
	if (message.dataType == MESSAGE::DATATYPES::ULONG)
	{
		std::cout << "Message: " << message.message << " Register: " << message._resgister << " Data: " << message.data << " Length: " << message.message.length() << " State: " << getResultString(message.state) << std::endl;
	}
	else
	{
		std::cout << "Message: " << message.message << " Register: " << message._resgister << " Data: " << message.special << " Length: " << message.message.length() << " State: " << getResultString(message.state) << std::endl;
	}	
}

int main()
{
	RESULT connectResult = ns.connect("\\\\.\\COM28");
	std::cout << getResultString(connectResult.state) << std::endl;
	ns.addOnReceiveHandler(&onRecv);
	ns.check();
	for (int i = 1; i < 50; i++)
	{
		std::cout << "\n";
		std::cout << "Iteration " << i << std::endl;
		while (!ns.getReady())
		{
			ns.check();
		}
		while (ns.getReady())
		{
			std::cout << "Was Ready " << ns.getReady() << std::endl;
			RESULT sendResult = ns.sendData(i, i);
			std::cout << "STATE: " << getResultString(sendResult.state) << " MESSAGE: " << sendResult.message << std::endl;
		}
	}
}