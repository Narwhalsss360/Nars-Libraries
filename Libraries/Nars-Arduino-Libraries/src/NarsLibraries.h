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

#define ZERO 0
#define NEWLINE '\n'
#define NULLTERMINATOR '\0'

#ifdef ARDUINO_ARCH_AVR
#define ULONG_MAX 4294967295 
#define LONG_MAX 2147483647
#define LONG_MIN -2147483648

#define UINT_MAX 65535
#define INT_MAX 32767
#define INT_MIN -32768

#define BYTE_MAX 255

#define ADCMAX 1023
#define PWNMAX 255
#endif // ARDUINO_ARCH_AVR

#define SERIALCOM_BAUD 1000000
#define LOG_MSG "LOG: ";
#define VERBOSE_MSG "VERBOSE: "
#define NOTICE_MSG "NOTICE: "
#define WARNING_MSG "WARNING: "
#define ERROR_MSG "ERROR: "

#define addInterrupt(pin, ISR, mode) attachInterrupt(digitalPinToInterrupt(pin), ISR, mode);
#define removeInterrupt(pin) detachInterrupt(digitalPinToInterrupt(pin));
#define BCD2DEC(num) hornerScheme(num, 0x10, 10)

unsigned long x2i(char*);

String toHex(unsigned long, byte);

int octalToDecimal(int);

long bitCast(unsigned long);

unsigned long bitCast(long);

double map(double, double, double, double, double);

unsigned long hornerScheme(unsigned long, unsigned long, unsigned long);

double intToFreq(double);

String boolToString(bool);

void byteWrite(byte, byte);

String boolToString(int);

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

	double convert(UNITS, double, const byte, const byte);
};

enum class TYPES : byte
{
	UND,
	BL,
	S8,
	S16,
	S32,
	U8,
	U16,
	U32,
	F32,
	D32
};

enum class LOGLEVEL
{
	VERBOSE,
	NOTICE,
	WARNING,
	ERROR
};

class NarsSerialCom
{
public:
	static unsigned long data[256];
	static bool connected;
	static void onSerialEvent(void (*) (unsigned int, unsigned long), void (*) (unsigned int, String));
	static void send(unsigned int, unsigned long);
	static void sendSpecial(unsigned int, String);
	static void sendDataRange(uint8_t, uint8_t);
	static void setReady();
	static void unsetReady();
private:
	static bool ready;
};

class PushToggle
{
public:
	PushToggle(byte, bool, byte);
	void read();
	bool toggled();
	void setCallBack(void (*) (void));
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

class Push
{
public:
	enum BUTTONSTATESINDEX
	{
		CURRENT,
		PRESS,
		RELEASE,
		PREVIOUS
	};
	Push(byte, bool, int);
	void update();
	bool current();
	bool pressed();
	bool released();
	unsigned int getHoldTime();
	void (*onRelease) (unsigned int);
	void (*onPress) (void);
private:
	enum member
	{
		PRESSEDMEMBER,
		RELEASEDMEMBER
	};
	const byte pin;
	const bool inverted;
	const unsigned short debounceDelay;
	bool state[4];
	unsigned long onPressTime;
	unsigned long lastDebounceTime;
	unsigned int holdTime;
	bool called[2];
};

class Logger
{
public:
	Logger();
	void verbose(const char* mes);
	void notice(const char* mes);
	void warning(const char* mes);
	void error(const char* mes);
	void verbose(const char* module, const char* mes);
	void notice(const char* module, const char* mes);
	void warning(const char* module, const char* mes);
	void error(const char* module, const char* mes);
	void verbose(String mes);
	void notice(String mes);
	void warning(String mes);
	void error(String mes);
	void verbose(String module, String mes);
	void notice(String module, String mes);
	void warning(String module, String mes);
	void error(String module, String mes);
	void addLocal(void (*_local)(Logger));
	void logAll();
	void directLog(TYPES, word, const char*);
	void addToLogger(TYPES, word, const char*);
	void removeFromLogger(word);
	void addOutput();
	void enableSerialOutput(bool);
	void setShowModule(bool);
	void setShowLevel(bool);
private:
	void (*local)(Logger) = 0;
	void (*output)(const char*) = 0 ;
	word registeredValues[16] = { 0 };
	TYPES types[16] = { TYPES::UND };
	char names[10][16];
	bool serialOutput = false;
	bool showModule;
	bool showLevel;
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
	void sendData(byte);
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
	byte getData(byte);
	void sendData(byte, byte);
	WireHostLite(byte);
private:
	byte notFoundCounter;
};

class WireClient
{
public:
	DevicePropertiesTemplateSlave deviceProperties;
	void onReceive(int);
	void onRequest();
	byte registerSelect;
	WireClient(byte, byte);
private:
	bool recv;
	int recvData;
};

#endif // TwoWire_h

extern NarsSerialCom SerialCom;

#define LIB_SIZE (sizeof(NarsSerialCom) + sizeof(PushToggle) + sizeof(UnitConverter) + sizeof(Logger))

#endif