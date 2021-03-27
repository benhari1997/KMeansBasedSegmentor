#pragma once
#include <cmath>
#include "generatorLibrary/prng.h"

/*
** XorshiftNG is a PrNG sub-class responsible for producing randomized 
** data based on the xorshift pseudo-random number generator method.
*/
class XorshiftNG : public PrNG
{
private:
	// An iterator taht is used to verify if the iterations number is
	// reached
	unsigned long m_period { 0 };
	// An intermediate variable used for the calcuation process,
	// and used to update the seed.
	long m_state { m_seed };

public:
	// Constructor
	XorshiftNG(int len, long seed)
		: PrNG(len, seed) {};
	// Overwritten PrNG iterate function in order to use the xorshift
	// method.
	long iterate();
};
