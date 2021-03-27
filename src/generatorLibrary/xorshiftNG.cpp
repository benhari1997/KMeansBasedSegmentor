#include "generatorLibrary/xorshiftNG.h"

long XorshiftNG::iterate() {
    do
    {
        m_state ^= m_state >> 7;
        m_state ^= m_state << 9;
        m_state ^= m_state >> 13;
        m_period++;
    } while (m_period <= (m_seed % 10)+1);
	m_seed = m_state;
    return m_state;
}
