#include "NarsUsefuls.h"

/// <summary>
/// map float values
/// </summary>
/// <param name="x">input</param>
/// <param name="in_min">input min</param>
/// <param name="in_max">input max</param>
/// <param name="out_min">output min</param>
/// <param name="out_max">output max</param>
/// <returns></returns>
double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/// <summary>
/// The Horner Scheme
/// </summary>
/// <param name="Num"></param>
/// <param name="Divider"></param>
/// <param name="Factor"></param>
/// <returns></returns>
unsigned long hornerScheme(unsigned long Num, unsigned long Divider, unsigned long Factor)
{
	unsigned long Remainder = 0, Quotient = 0, Result = 0;
	Remainder = Num % Divider;
	Quotient = Num / Divider;
	if (!(Quotient == 0 && Remainder == 0))
		Result += hornerScheme(Quotient, Divider, Factor) * Factor + Remainder;
	return Result;
}