#ifndef NarsLibraries
#define NarsLibraries

#if __has_include ("Wire.h")
#include "Wire.h"
#endif // __has_include ("Wire.h")

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define LONG_MAX 2147483647

#define BCD2DEC(num) HornerScheme(num, 0x10, 10)

unsigned long x2i(char* s);

String toHex(unsigned long input, byte stringLength);

int octalToDecimal(int n);

long bitCast(unsigned long in);

unsigned long bitCast(long in);

double mapValue(double x, double in_min, double in_max, double out_min, double out_max);

unsigned long HornerScheme(unsigned long Num, unsigned long Divider, unsigned long Factor);

double INT2FREQ(unsigned long input);

class NarsSerialCom
{
public:
	static unsigned long data[256];
	static bool connected;
	static void onSerialEvent(void (*done) (unsigned int _register, unsigned long data), void (*special) (unsigned int registerrr, String data));
	static void send(unsigned int _register, unsigned long data);
	static void sendSpecial(unsigned int _register, String data);
};

String wireSearch();

#if defined(TwoWire_h)
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

#endif // TwoWire_h

extern NarsSerialCom SerialCom;
#endif

