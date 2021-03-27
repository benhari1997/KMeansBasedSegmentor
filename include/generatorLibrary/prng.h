#pragma once
#include <cmath>
/*
** PrNG is the class responsible for generating random <double> numbers 
** based on the chosen random generator subclass and according to the 
** specified seed (<double> seed), and output number length (<int> len)
** 
** Example :
** PrNG *randClass = new PrNG(seed, length);
** double generatedNumber = randClass->rand();
*/
class PrNG {
protected:
	//stores the specified output random numbers length
    int m_len;
	// used by the PrNG subclasses in the random number
	// generation process.
    long m_seed;

public:
    // Constructors
    PrNG(int, long);

    // Seed acc/mutator
    void setSeed(long);
    long getSeed() const;

    // Len acc/mutator
    void setLen(int);
    int getLen() const;

	// Applies a series of operations on the seed,
	// updates its value and returns it.
    virtual long iterate();

	// Takes the output value of the iterator and
	// transforms it into a valid length double.
    double rand();
};
