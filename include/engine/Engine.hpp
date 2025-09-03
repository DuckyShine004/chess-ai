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

    void run();

  private:
    engine::board::Board _board;

    // Get all pseudo-legal moves
    std::vector<engine::move::Move> getMoves(engine::board::Colour side);

    std::vector<engine::move::Move> getLegalMoves();

    std::vector<engine::move::Move> getQuietMoves(engine::board::Colour side);

    std::vector<engine::move::Move> getCaptureMoves(engine::board::Colour side);

    std::vector<engine::move::Move> getPawnQuietMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getPawnCaptureMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getKnightQuietMoves(int square, engine::board::Colour colour);

    std::vector<engine::move::Move> getKnightCaptureMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getBishopQuietMoves(int square, engine::board::Colour colour);

    std::vector<engine::move::Move> getBishopCaptureMoves(int square, engine::board::Colour side);

    void addToMoves(std::vector<engine::move::Move> &moves, std::vector<engine::move::Move> &&otherMoves);

    bool isMoveLegal(engine::move::Move &move, engine::board::Colour side);

    void makeMove(engine::move::Move &move);

    void unmakeMove(engine::move::Move &move);

    int quiescence(int alpha, int beta);

    int evaluate();

    int getMaterialScore();

    int getMobilityScore();
};

} // namespace engine