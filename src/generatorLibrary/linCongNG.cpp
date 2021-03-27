#include <cmath>
#include "generatorLibrary/linCongNG.h"

long LinCongNG::iterate() {
	m_seed = (m_seed * m_mult + m_increment) % m_mod;
    return m_seed;
}
