// C++ Library Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
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
	std::cout << "Message: " << message.message << " Register: " << message._resgister << " Data: " << message.data << " Length: " << message.message.length() << " State: " << getResultString(message.state) << std::endl;
}

int main()
{
	RESULT connectResult = ns.connect("\\\\.\\COM28");
	std::cout << getResultString(connectResult.state) << std::endl;
	ns.addOnReceiveHandler(&onRecv);
	while (true)
	{
		RESULT checkResult = ns.check();
	}
}