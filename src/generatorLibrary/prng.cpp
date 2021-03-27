#include "generatorLibrary/prng.h"

PrNG::PrNG(int len, long seed)
    : m_seed { seed }
    , m_len { len } {};

long PrNG::iterate() { return m_seed; }

double PrNG::rand()
{
    long intpart = iterate() % (long)(pow(10, m_len) - 1);
    double realpart = intpart / (pow(10, m_len) - 1);
    return intpart + realpart;
}

long PrNG::getSeed() const { return m_seed; }
void PrNG::setSeed(long seed) { m_seed = seed; }

int PrNG::getLen() const { return m_len; }
void PrNG::setLen(int len) { m_len = len; }
