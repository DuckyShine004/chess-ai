#include <cstdlib>

#include "utility/RandomUtility.hpp"

namespace utility {

// Reduce to avoid hash collision
// https://github.com/lemire/zobristhashing/blob/master/src/zobrist.c
uint64_t RandomUtility::getRandomU64() {
    uint64_t h0 = (((uint64_t)rand() << 0) & 0x000000000000FFFFULL);
    uint64_t h1 = (((uint64_t)rand() << 16) & 0x00000000FFFF0000ULL);
    uint64_t h2 = (((uint64_t)rand() << 32) & 0x0000FFFF00000000ULL);
    uint64_t h3 = (((uint64_t)rand() << 48) & 0xFFFF000000000000ULL);

    return h0 | h1 | h2 | h3;
}

} // namespace utility