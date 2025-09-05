#pragma once

#include <cstdint>

#include "engine/board/Board.hpp"

namespace engine::hash {

struct Key {
    uint64_t piece[64][12];
    uint64_t side;
};

class Zobrist {
  public:
    Zobrist(uint64_t seed);

    uint64_t hash(engine::board::Board &board);

    Key &getKey();

  private:
    Key _key;
};

} // namespace engine::hash