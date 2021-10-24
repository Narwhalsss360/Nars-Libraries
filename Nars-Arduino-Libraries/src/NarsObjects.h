#ifndef NarsObjects
#define NarsObjects

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

unsigned long x2i(char* s);
String toHex(unsigned long input, byte stringLength);
static int octalToDecimal(int n);

struct DevicePropertiesTemplateSlave
{
	byte id;
	byte adress;
	byte data[256];
};

struct DevicePropertiesTemplateMaster
{
	struct StatesTemplate
	{
		enum DeviceStatus
		{
			CONNECTED,
			NOT_FOUND,
			DISCONNECTED
		};
	} states;
	byte id;
	byte address;
	byte data[256];
	int state;
};

class NarsSerialCom
{
public:
	static unsigned long data[512];
	static bool connected;
	static void onSerialEvent(void (*done) (int registerrr, unsigned long data), void (*special) (int registerrr, String data));
	static void onSerialEvent(void (*special) (int _register, String data));
};

class WireHost
{
public:
	DevicePropertiesTemplateMaster deviceProperties;
	void check();
	void getData();
	void sendData();
	void sendData(byte addr);
private:
	byte notFoundCounter;
};

class WireClient
{
public:
	DevicePropertiesTemplateSlave deviceProperties;
	void onReceive(int bytes);
	void onRequest();
	byte registerSelect;
private:
	bool recv;
	byte recvData;
};
#endif

