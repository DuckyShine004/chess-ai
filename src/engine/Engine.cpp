#include <limits>
#include <iostream>

#include "engine/Engine.hpp"

using namespace engine::board;

using namespace engine::move;

namespace engine {

Engine::Engine() {
    // std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR - - - - -";
    std::string fen = "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2";

    this->_board = Board(fen);

    Move move;

    move.from = this->_board.getSquareFromPosition("c2");
    move.to = this->_board.getSquareFromPosition("c6");

    move.piece = this->_board.getPieceFromSquare(move.from);
    move.colour = this->_board.getColourFromSquare(move.from);

    this->makeMove(move);
    // this->unmakeMove(move);
}

Board &Engine::getBoard() {
    return this->_board;
}

std::vector<Move> Engine::getMoves() {
    return {};
}

std::vector<Move> Engine::getLegalMoves() {
    return {};
}

std::vector<Move> Engine::getQuietMoves() {
    return {};
}

void Engine::makeMove(Move &move) {
    this->_board.removeFromSquare(move.from);
    this->_board.createPieceInSquare(move.to, move.piece, move.colour);
}

void Engine::unmakeMove() {
}

// Implement null heuristic TODO
int Engine::search(int alpha, int beta, int depth) {
    // Perform quiescence search after
    if (depth == 0) {
        return this->quiescence(alpha, beta);
    }

    int bestScore = std::numeric_limits<int>::min();

    // Get all legal moves

    // For all moves: TODO
    // make move
    int score = -this->search(-beta, -alpha, depth - 1);
    // unmake move

    if (score > bestScore) {
        bestScore = score;

        if (score > alpha) {
            alpha = score;
        }

        if (score >= beta) {
            return bestScore;
        }
    }

    return bestScore;
}

int Engine::quiescence(int alpha, int beta) {
    int evaluation = this->evaluate();

    // Standing pat- allows quiescence to stabilise
    int bestScore = this->evaluate();

    if (bestScore >= beta) {
        return bestScore;
    }

    if (bestScore > alpha) {
        alpha = bestScore;
    }

    // Examine all captures: TODO
    // Make capture: TODO
    int score = -this->quiescence(-beta, -alpha);
    // Take back capture: TODO
    if (score >= beta) {
        return score;
    }

    if (score > bestScore) {
        bestScore = score;
    }

    if (score > alpha) {
        alpha = score;
    }

    return score;
}

int Engine::evaluate() {
    int materialScore = this->getMaterialScore();
    int mobilityScore = this->getMobilityScore();

    int score = materialScore + mobilityScore;
    int sideToMove = 1; // White = +1, Black=-1

    return score * sideToMove;
}

// Note to self: using Negamax framework
int Engine::getMaterialScore() {
    return 1;
}

int Engine::getMobilityScore() {
    return 1;
}

} // namespace engine