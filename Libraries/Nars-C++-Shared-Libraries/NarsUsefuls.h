#pragma once

#define NarsUsefuls_h
#define ZERO 0
#define NEWLINE '\n'
#define CARRIAGERETURN '\r'
#define NULLTERMINATOR '\0'
#define SERIALCOM_BAUD 1000000
#define reinterpret_c(source, destType) *(destType*)&source
#define BCD2DEC(num) hornerScheme(num, 0x10, 10)

#ifdef __cplusplus
extern "C" 
{
#endif // __cplusplus
	double mapf(double, double, double, double, double);

	unsigned long hornerScheme(unsigned long, unsigned long, unsigned long);
#ifdef __cplusplus
}
#endif // __cplusplus

#ifndef __cpluscplus
	double mapf(double, double, double, double, double);

	unsigned long hornerScheme(unsigned long, unsigned long, unsigned long);
#endif // !__cpluscplus
