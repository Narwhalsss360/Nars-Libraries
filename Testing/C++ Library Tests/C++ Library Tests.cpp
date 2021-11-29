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
	case RESULT::STATES::NONE: return "NONE";
	case RESULT::STATES::CONNECTED: return "CONNECTED";
	case RESULT::STATES::ALREADY_CONNECTED: return "ALREADY_CONNECTED";
	case RESULT::STATES::DISCONNECTED: return "DISCONNECTED";
	case RESULT::STATES::ALREADY_DISCONNECTED: return "ALREADY_DISCONNECTED";
	case RESULT::STATES::OUT_OF_RANGE: return "OUT_OF_RANGE";
	case RESULT::STATES::LOST: return "LOST";
	case RESULT::STATES::SENT: return "SENT";
	case RESULT::STATES::RECEIVED: return "RECEIVED";
	case RESULT::STATES::PARSED: return "PARSED";
	case RESULT::STATES::NOT_READY: return "NOT_READY";
	case RESULT::STATES::QUEUED: return "QUEUED";
	default: return "n/a";
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
	ns.addOnReceiveHandler(&onRecv);
	RESULT result = ns.connect("\\\\.\\COM28");
	Sleep(1000);
	std::cout << "Message: " << result.message << getResultString(result.state) << std::endl;
	result = ns.check();
	std::cout << "Message: " << result.message << getResultString(result.state) << std::endl;
	result = ns.checkQueue();
	std::cout << "Message: " << result.message << getResultString(result.state) << std::endl;
	result = ns.sendData(978, 980);
	std::cout << "Message: " << result.message << getResultString(result.state) << std::endl;
	return 0;
}