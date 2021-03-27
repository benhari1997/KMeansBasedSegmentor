#include "generatorLibrary/mersTwistNG.h"

MersTwistNG::MersTwistNG(int len, long seed)
    : PrNG(len, seed)
{
    m_state[0] = m_seed;
    for (size_t i = 1; i < 624; i++) {
        m_state[i] = m_f * (m_state[i - 1] ^ (m_state[i - 1] >> 30)) + i;
        m_state[i] = m_state[i] & 0xFFFFFFFF;
    }
}

void MersTwistNG::twist()
{
    for (size_t i = 0; i < 624; i++) {
        uint32_t temp, temp_shift;
        temp = (m_state[i] & m_upper_mask) + (m_state[(i + 1) % 624] & m_lower_mask);
        temp = temp & 0xFFFFFFFF;

        temp_shift = temp >> 1;
        if (temp % 2 != 0) {
            temp_shift = temp_shift ^ 0x9908b0df;
        };

        m_state[i] = m_state[(i + m_m) % 624] ^ temp_shift;
    }
    m_index = 0;
}

long MersTwistNG::iterate()
{
    if (m_index >= 624) {
        twist();
    };
    m_index++;

    uint32_t randNum { m_state[m_index] };
    randNum = randNum ^ (randNum >> m_u);
    randNum = randNum ^ ((randNum << m_s) & m_b);
    randNum = randNum ^ ((randNum << m_t) & m_c);
    randNum = randNum ^ (randNum >> m_l);

    return 0xFF & randNum;
}
