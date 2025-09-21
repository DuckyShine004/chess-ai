#pragma once

#include <vector>
#include <random>
#include <cstdint>

namespace utility::RandomUtility {

inline std::random_device randomDevice;

inline std::mt19937 generator(randomDevice());

inline uint32_t state = 1804289383U;

[[nodiscard]] inline uint32_t getRandomU32();

[[nodiscard]] inline uint64_t getRandomU64();

template <typename T> [[nodiscard]] T getRandomElement(std::vector<T> &vec);

[[nodiscard]] inline uint32_t getRandomU32() {
    uint32_t x = state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    return state = x;
}

[[nodiscard]] inline uint64_t getRandomU64() {
    uint64_t h1 = (uint64_t)(getRandomU32()) & 0xFFFF;
    uint64_t h2 = (uint64_t)(getRandomU32()) & 0xFFFF;
    uint64_t h3 = (uint64_t)(getRandomU32()) & 0xFFFF;
    uint64_t h4 = (uint64_t)(getRandomU32()) & 0xFFFF;

    return h1 | (h2 << 16) | (h3 << 32) | (h4 << 48);
}

template <typename T> [[nodiscard]] T getRandomElement(std::vector<T> &vec) {
    std::uniform_int_distribution<> distribution(0, vec.size() - 1);

    int index = distribution(generator);

    return vec[index];
}

} // namespace utility::RandomUtility
