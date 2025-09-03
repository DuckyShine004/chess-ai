#include <limits>
#include <iostream>

#include "engine/Engine.hpp"

using namespace engine::board;

using namespace engine::move;

namespace engine {

Engine::Engine() {
    // std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR - - - - -";
    // std::string fen = "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2";
    std::string fen = "rnbqkbnr/pppppppp/8/8/3N4/8/PPPPPPPP/R1BQKBNR w KQkq - 0 1";

    this->_board = Board(fen);
}

void Engine::run() {
    std::vector<Move> quietMoves = this->getQuietMoves(Colour::WHITE);
    std::vector<Move> captureMoves = this->getCaptureMoves(Colour::WHITE);

    // for (Move &move : quietMoves) {
    //     makeMove(move);
    //     this->_board.print();
    //     unmakeMove(move);
    // }
    // for (Move &move : captureMoves) {
    //     makeMove(move);
    //     this->_board.print();
    //     unmakeMove(move);
    // }
}

Board &Engine::getBoard() {
    return this->_board;
}

std::vector<Move> Engine::getMoves(Colour side) {
    return {};
}

std::vector<Move> Engine::getLegalMoves() {
    return {};
}

std::vector<Move> Engine::getQuietMoves(Colour side) {
    std::vector<Move> moves;

    for (int square = 0; square < 64; ++square) {
        Colour colour = this->_board.getColourFromSquare(square);

        if (colour != side) {
            continue;
        }

        Piece piece = this->_board.getPieceFromSquare(square);

        switch (piece) {
        case Piece::KNIGHT:
            this->addToMoves(moves, this->getKnightQuietMoves(square, side));
            break;
        default:
            break;
        }
    }

    return moves;
}

std::vector<Move> Engine::getCaptureMoves(Colour side) {
    std::vector<Move> moves;

    for (int square = 0; square < 64; ++square) {
        Colour colour = this->_board.getColourFromSquare(square);

        if (colour != side) {
            continue;
        }

        Piece piece = this->_board.getPieceFromSquare(square);

        switch (piece) {
        case Piece::KNIGHT:
            this->addToMoves(moves, this->getKnightCaptureMoves(square, side));
            break;
        default:
            break;
        }
    }

    return moves;
}

// Pre compute table if needed
std::vector<Move> Engine::getKnightQuietMoves(int square, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 8; ++i) {
        int toRank = rank + KNIGHT_MOVES[i][0];
        int toFile = file + KNIGHT_MOVES[i][1];

        if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
            continue;
        }

        int toSquare = this->_board.getSquare(toRank, toFile);

        if (this->_board.getPieceFromSquare(toSquare) != Piece::EMPTY) {
            continue;
        }

        Move move(square, toSquare, Piece::KNIGHT, side);

        if (this->isMoveLegal(move, side)) {
            moves.push_back(move);
        }
    }

    return moves;
}

std::vector<Move> Engine::getKnightCaptureMoves(int square, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 8; ++i) {
        int toRank = rank + KNIGHT_MOVES[i][0];
        int toFile = file + KNIGHT_MOVES[i][1];

        if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
            continue;
        }

        int toSquare = this->_board.getSquare(toRank, toFile);

        Piece toPiece = this->_board.getPieceFromSquare(toSquare);
        Colour toColour = this->_board.getColourFromSquare(toSquare);

        if (toPiece == Piece::EMPTY || toColour == side) {
            continue;
        }

        Move move(square, toSquare, Piece::KNIGHT, side);

        move.capturedPiece = toPiece;

        if (this->isMoveLegal(move, side)) {
            moves.push_back(move);
        }
    }

    return moves;
}

void Engine::addToMoves(std::vector<Move> &moves, std::vector<Move> &&otherMoves) {
    if (moves.empty()) {
        moves = std::move(otherMoves);
    } else {
        moves.reserve(moves.size() + otherMoves.size());
        moves.insert(moves.end(), std::make_move_iterator(otherMoves.begin()), std::make_move_iterator(otherMoves.end()));
    }
}

bool Engine::isMoveLegal(Move &move, Colour side) {
    // Make the move then check if current side king is in check
    bool isMoveLegal = true;

    this->makeMove(move);

    Colour otherSide = static_cast<Colour>(side ^ 1);

    std::vector<Move> enemyMoves = this->getCaptureMoves(otherSide);

    for (Move &move : enemyMoves) {
        if (move.capturedPiece == Piece::KING) {
            isMoveLegal = false;

            break;
        }
    }

    this->unmakeMove(move);

    return isMoveLegal;
}

void Engine::makeMove(Move &move) {
    this->_board.removeFromSquare(move.from);
    this->_board.createPieceInSquare(move.to, move.piece, move.colour);
}

void Engine::unmakeMove(Move &move) {
    this->_board.removeFromSquare(move.to);

    this->_board.createPieceInSquare(move.from, move.piece, move.colour);

    // If not empty, place back the captured piece in the to square
    if (move.capturedPiece != Piece::EMPTY) {
        Colour otherColour = static_cast<Colour>(move.colour ^ 1);

        this->_board.createPieceInSquare(move.to, move.capturedPiece, otherColour);
    }
}

// Implement null heuristic TODO
int Engine::search(int alpha, int beta, int depth) {
    // Perform quiescence search after
    if (depth == 0) {
        return this->quiescence(alpha, beta);
    }

    int bestScore = std::numeric_limits<int>::min();

    // Get all legal moves
    std::vector<Move> moves = this->getLegalMoves();

    for (Move &move : moves) {
        this->makeMove(move);

        int score = -this->search(-beta, -alpha, depth - 1);

        this->unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;

            if (score > alpha) {
                alpha = score;
            }
        }

        if (score >= beta) {
            return bestScore;
        }
    }

    return bestScore;
}

int Engine::quiescence(int alpha, int beta) {
    int bestScore = this->evaluate();

    if (bestScore >= beta) {
        return bestScore;
    }

    if (bestScore > alpha) {
        alpha = bestScore;
    }

    std::vector<Move> captures = this->getCaptureMoves(Colour::WHITE);

    for (Move &capture : captures) {
        this->makeMove(capture);

        int score = -this->quiescence(-beta, -alpha);

        this->unmakeMove(capture);

        if (score >= beta) {
            return score;
        }

        if (score > bestScore) {
            bestScore = score;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return bestScore;
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