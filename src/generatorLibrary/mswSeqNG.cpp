#include <cmath>
#include "generatorLibrary/mswSeqNG.h"

long MswSecNG::iterate() {
    m_x *= m_x;
    m_x += (m_w += m_seed);
    m_x = (m_x >> 32) | (m_x << 32);
    return m_x;
}
