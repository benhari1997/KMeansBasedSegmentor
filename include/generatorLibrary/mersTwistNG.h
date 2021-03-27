#pragma once
#include "generatorLibrary/prng.h"
#include <cstdint>

/*
** MersTwistNG is a PrNG sub-class responsible for producing randomized
** data based on the mersenne twister pseudo-random number generator method.
*/
class MersTwistNG : public PrNG {
private:
	// Intermediate variables used to randomise the calculations
	// (part of the Mersenne twister method)
    uint32_t m_state[624];
    uint32_t m_f{ 1812433253 },
        m_b{ 0x9D2C5680 },
        m_c{ 0xEFC60000 };
    unsigned long m_lower_mask = (1 << 31) - 1l,
        m_upper_mask = 1 << 31;
    int m_m { 397 },
        m_u { 11 },
        m_s { 7 },
        m_t { 15 },
        m_l { 18 },
        m_index { 624 },
        m_intSize;
	// Changes the random numbers matrix after 624 sample to prevent 
	// duplicates.
    void twist();

public:
	// Constructor
    MersTwistNG(int len, long seed);
	// Overwritten PrNG iterate function in order to use the 'MSW
	// Sequence' method.
    long iterate();
};
