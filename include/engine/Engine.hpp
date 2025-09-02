#pragma once

#include <vector>

#include "engine/board/Board.hpp"

#include "engine/move/Move.hpp"

namespace engine {

class Engine {
  public:
    Engine();

    engine::board::Board &getBoard();

    int search(int alpha, int beta, int depth);

  private:
    engine::board::Board _board;

    // Get all pseudo-legal moves
    std::vector<engine::move::Move> getMoves();

    std::vector<engine::move::Move> getLegalMoves();

    std::vector<engine::move::Move> getQuietMoves();

    void makeMove(engine::move::Move &move);

    void unmakeMove();

    int quiescence(int alpha, int beta);

    int evaluate();

    int getMaterialScore();

    int getMobilityScore();
};

} // namespace engine