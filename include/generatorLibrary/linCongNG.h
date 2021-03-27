#pragma once
#include <math.h>
#include "generatorLibrary/prng.h"

/*
** LinCongNG is a PrNG sub-class responsible for producing randomized
** data based on the Linear congruent pseudo-random number generator method.
*/
class LinCongNG : public PrNG {
private:
	// Intermediate variables used to randomise the calculations
	// (part of the MSW Sequence method)
    long m_mult { 1664525 },
        m_increment { 1013904223 },
        m_mod { (long)exp2(32) };

public:
	// Constructor
    LinCongNG(int len, long seed)
        : PrNG(len, seed) {};
	// Overwritten PrNG iterate function in order to use the 'MSW
	// Sequence' method.
    long iterate();
};
