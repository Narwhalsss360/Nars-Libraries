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

#define ULONG_MAX 4294967295 
#define LONG_MAX 2147483647
#define LONG_MIN -2147483648

#define UINT_MAX 65535
#define INT_MAX 32767
#define INT_MIN -32768

#define BCD2DEC(num) hornerScheme(num, 0x10, 10)

unsigned long x2i(char* s);

String toHex(unsigned long input, byte stringLength);

int octalToDecimal(int n);

long bitCast(unsigned long in);

unsigned long bitCast(long in);

double mapValue(double x, double in_min, double in_max, double out_min, double out_max);

unsigned long hornerScheme(unsigned long Num, unsigned long Divider, unsigned long Factor);

double intToFreq(double input);

String boolToString(bool input);

void byteWrite(byte pin, byte byteOut);

String boolToString(int input);

struct UnitConverter
{
	enum class UNITS : byte
	{
		LENGTH,
		VOLUME,
		TIME,
		TEMPERATURE,
		MASS,
		LUMOSITY
	};

	enum class LENGTH : byte
	{
		picometer,
		nanometer,
		micrometer,
		millimeter,
		centimeter,
		decimeter,
		meter,
		dekameter,
		hectometer,
		kilometer,
		megameter,
		gigameter,
		terameter,
		petameter,
		inch,
		foot,
		yard,
		mile,
		lightyear
	};

	enum class VOLUME : byte
	{
		picoliter,
		nanoliter,
		microliter,
		milliliter,
		centiliter,
		deciliter,
		liter,
		dekaliter,
		hectoliter,
		kiloliter,
		megaliter,
		gigaliter,
		teraliter,
		petaliter,
		ounce,
		pint,
		gallon
	};

	enum class TIME : byte
	{
		nanosecond,
		millisecond,
		second,
		minute,
		hour,
		day,
		week,
		month,
		year,
		decade,
		century
	};

	enum class TEMPERATURE : byte
	{
		kelvin,
		Celcius,
		Fahrenheit
	};

	enum class MASS : byte
	{
		picogram,
		nanogram,
		microgram,
		milligram,
		centigram,
		decigram,
		gram,
		dekagram,
		hectogram,
		kilogram,
		tonne,
		ounce,
		pound,
		ton
	};

	enum class LUMOSITY : byte
	{
		candela,
		lumens
	};

	double result;
	double original;
	UNITS originalUnit;
	UNITS resultUnit;

	double convert(UNITS unitType, double input, const byte inputUnit, const byte outputUnit);
};

class NarsSerialCom
{
public:
	static unsigned long data[256];
	static bool connected;
	static void onSerialEvent(void (*done) (unsigned int _register, unsigned long data), void (*special) (unsigned int registerrr, String data));
	static void send(unsigned int _register, unsigned long data);
	static void sendSpecial(unsigned int _register, String data);
	static void sendDataRange(uint8_t start, uint8_t end);
	static void setReady();
	static void unsetReady();
private:
	static bool ready;
};

class PushToggle
{
public:
	PushToggle(byte _pin, bool _invert, byte _debounceDelay);
	void read();
	bool toggled();
	void setCallBack(void (*_callBack) (void));
private:
	byte pin;
	bool invertedInput;
	byte debounceDelay;
	unsigned long lastDebounceTime;
	bool lastButtonState;
	bool endState;
	bool toggle;
	void (*callBack) (void);
};

#if defined(TwoWire_h)

struct WireSearch
{
	bool availableWireAddresses[127];
	void search();
};

struct DevicePropertiesTemplateSlave
{
	byte id;
	byte address;
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

class WireHostLite
{
public:
	bool connected;
	byte id;
	byte address;
	void check();
	byte getData(byte dataRegister);
	void sendData(byte dateRegister, byte data);
	WireHostLite(byte _address);
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
	WireClient(byte address, byte id);
private:
	bool recv;
	int recvData;
};

#endif // TwoWire_h

extern NarsSerialCom SerialCom;

#define LIB_SIZE (sizeof(NarsSerialCom) + sizeof(PushToggle) + sizeof(UnitConverter))

#endif