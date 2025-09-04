#pragma once

#include <vector>
#include <climits>

#include "engine/board/Board.hpp"

#include "engine/move/Move.hpp"

namespace engine {

struct Result {
    int bestScore;

    engine::move::Move bestMove;

    bool isMoveFound;

    Result() {
        this->bestScore = -INT_MAX;

        isMoveFound = false;
    }
};

class Engine {
  public:
    Engine();

    engine::board::Board &getBoard();

    void run();

    Result searchRoot(int depth);

  private:
    engine::board::Board _board;

    std::vector<engine::move::Move> getMoves(engine::board::Colour side);

    std::vector<engine::move::Move> getLegalMoves(engine::board::Colour side);

    std::vector<engine::move::Move> getQuietMoves(engine::board::Colour side);

    std::vector<engine::move::Move> getCaptureMoves(engine::board::Colour side);

    std::vector<engine::move::Move> getPawnQuietMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getPawnCaptureMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getKnightQuietMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getKnightCaptureMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getBishopQuietMoves(int square, engine::board::Piece sliderPiece, engine::board::Colour side);

    std::vector<engine::move::Move> getBishopCaptureMoves(int square, engine::board::Piece sliderPiece, engine::board::Colour side);

    std::vector<engine::move::Move> getRookQuietMoves(int square, engine::board::Piece sliderPiece, engine::board::Colour side);

    std::vector<engine::move::Move> getRookCaptureMoves(int square, engine::board::Piece slidePiece, engine::board::Colour side);

    std::vector<engine::move::Move> getQueenQuietMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getQueenCaptureMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getKingQuietMoves(int square, engine::board::Colour side);

    std::vector<engine::move::Move> getKingCaptureMoves(int square, engine::board::Colour side);

    void addToMoves(std::vector<engine::move::Move> &moves, std::vector<engine::move::Move> &&otherMoves);

    bool isMoveLegal(engine::move::Move &move);

    void makeMove(engine::move::Move &move);

    void unmakeMove(engine::move::Move &move);

    int search(int alpha, int beta, int depth);

    int quiescence(int alpha, int beta);

    int evaluate(engine::board::Colour side);

    int getMaterialScore(engine::board::Colour side);

    int getPositionScore(engine::board::Colour side);
};

} // namespace engine