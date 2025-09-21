#pragma once

#include <cstddef>
#include <cstdint>

namespace engine::hash::Transposition {

enum NodeType : uint8_t {
    ALPHA = 0,
    BETA = 1,
    EXACT = 2,
    UNKNOWN = 3,
};

struct Entry {
    uint64_t zobrist;

    int32_t score;
    int16_t depth;

    uint16_t bestMove;

    NodeType nodeType;

    Entry() : zobrist(0ULL), score(0), depth(0), nodeType(NodeType::UNKNOWN) {
    }
};

// 2 MB transposition table = 2mb / 32b entries
inline constexpr size_t TRANSPOSITION_TABLE_BYTES = 2ULL << 20;
inline constexpr size_t TRANSPOSITION_TABLE_ENTRIES = TRANSPOSITION_TABLE_BYTES / sizeof(Entry);
inline constexpr size_t TRANSPOSITION_TABLE_MASK = TRANSPOSITION_TABLE_ENTRIES - 1;

} // namespace engine::hash::Transposition
