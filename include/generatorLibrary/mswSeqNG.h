#pragma once
#include "generatorLibrary/prng.h"

/*
** MswSecNG is a PrNG sub-class responsible for producing randomized
** data based on the MSW Sequence pseudo-random number generator method.
*/
class MswSecNG : public PrNG {
private:
	// Intermediate variables used to randomise the calculations
	// (part of the MSW Sequence method)
    long m_x { 0 }, m_w { 0 };

public:
	// Constructor
    MswSecNG(int len, long seed)
        : PrNG(len, seed) {};
	// Overwritten PrNG iterate function in order to use the 'MSW
	// Sequence' method.
    long iterate();
};
