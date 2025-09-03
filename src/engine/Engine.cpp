#include <limits>
#include <iostream>

#include "engine/Engine.hpp"

using namespace engine::board;

using namespace engine::move;

namespace engine {

Engine::Engine() {
    // std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR - - - - -";
    // std::string fen = "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2";
    // std::string fen = "rnbqkbnr/pppppppp/8/8/2N5/1P1K4/P1PPPPPP/R1BQ1BNR w KQkq - 0 1";
    std::string fen = "8/8/8/4K3/8/8/8/8 w KQkq - 0 1";

    this->_board = Board(fen);
}

void Engine::run() {
    std::vector<Move> quietMoves = this->getQuietMoves(Colour::WHITE);
    // std::vector<Move> captureMoves = this->getCaptureMoves(Colour::WHITE);

    for (Move &move : quietMoves) {
        makeMove(move);
        this->_board.print();
        unmakeMove(move);
    }
    // for (Move &move : captureMoves) {
    //     makeMove(move);
    //     this->_board.print();
    //     unmakeMove(move);
    // }
    // this->_board.print();
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
        case Piece::PAWN:
            this->addToMoves(moves, this->getPawnQuietMoves(square, side));
            break;
        case Piece::KNIGHT:
            this->addToMoves(moves, this->getKnightQuietMoves(square, side));
            break;
        case Piece::BISHOP:
            this->addToMoves(moves, this->getBishopQuietMoves(square, Piece::BISHOP, side));
            break;
        case Piece::ROOK:
            this->addToMoves(moves, this->getRookQuietMoves(square, Piece::ROOK, side));
            break;
        case Piece::QUEEN:
            this->addToMoves(moves, this->getQueenQuietMoves(square, side));
            break;
        case Piece::KING:
            this->addToMoves(moves, this->getKingQuietMoves(square, side));
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
        case Piece::PAWN:
            this->addToMoves(moves, this->getPawnCaptureMoves(square, side));
            break;
        case Piece::KNIGHT:
            this->addToMoves(moves, this->getKnightCaptureMoves(square, side));
            break;
        case Piece::BISHOP:
            this->addToMoves(moves, this->getBishopCaptureMoves(square, Piece::BISHOP, side));
            break;
        case Piece::ROOK:
            this->addToMoves(moves, this->getRookCaptureMoves(square, Piece::ROOK, side));
            break;
        case Piece::QUEEN:
            this->addToMoves(moves, this->getQueenCaptureMoves(square, side));
            break;
        case Piece::KING:
            this->addToMoves(moves, this->getKingCaptureMoves(square, side));
            break;
        default:
            break;
        }
    }

    return moves;
}

std::vector<Move> Engine::getPawnQuietMoves(int square, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 2; ++i) {
        int toRank = rank + PAWN_MOVES[side][i];
        int toFile = file;

        if (toRank < 0 || toRank >= 8) {
            break;
        }

        int toSquare = this->_board.getSquare(toRank, toFile);

        if (this->_board.getPieceFromSquare(toSquare) != Piece::EMPTY) {
            break;
        }

        if (i == 0) {
            moves.emplace_back(square, toSquare, Piece::PAWN, side);
        } else {
            if (rank == SECOND_RANK[side]) {
                moves.emplace_back(square, toSquare, Piece::PAWN, side);
            }
        }
    }

    return moves;
}

std::vector<Move> Engine::getPawnCaptureMoves(int square, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 2; ++i) {
        int toRank = rank + PAWN_ATTACKS[side][i][0];
        int toFile = file + PAWN_ATTACKS[side][i][1];

        if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
            break;
        }

        int toSquare = this->_board.getSquare(toRank, toFile);

        Piece toPiece = this->_board.getPieceFromSquare(toSquare);
        Colour toColour = this->_board.getColourFromSquare(toSquare);

        if (toPiece == Piece::EMPTY || toColour == side) {
            continue;
        }

        Move move(square, toSquare, Piece::PAWN, side);

        move.capturedPiece = toPiece;

        moves.push_back(move);
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

        moves.emplace_back(square, toSquare, Piece::KNIGHT, side);
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

        moves.push_back(move);
    }

    return moves;
}

std::vector<Move> Engine::getBishopQuietMoves(int square, Piece sliderPiece, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 4; ++i) {
        int coefficient = 1;

        while (true) {
            int toRank = rank + coefficient * BISHOP_MOVES[i][0];
            int toFile = file + coefficient * BISHOP_MOVES[i][1];

            if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
                break;
            }

            int toSquare = this->_board.getSquare(toRank, toFile);

            if (this->_board.getPieceFromSquare(toRank, toFile) != Piece::EMPTY) {
                break;
            }

            moves.emplace_back(square, toSquare, sliderPiece, side);

            ++coefficient;
        }
    }

    return moves;
}

std::vector<Move> Engine::getBishopCaptureMoves(int square, Piece sliderPiece, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 4; ++i) {
        int coefficient = 1;

        while (true) {
            int toRank = rank + coefficient * BISHOP_MOVES[i][0];
            int toFile = file + coefficient * BISHOP_MOVES[i][1];

            if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
                break;
            }

            int toSquare = this->_board.getSquare(toRank, toFile);

            Piece toPiece = this->_board.getPieceFromSquare(toSquare);
            Colour toColour = this->_board.getColourFromSquare(toSquare);

            if (toPiece == Piece::EMPTY) {
                ++coefficient;

                continue;
            }

            if (toColour == side) {
                break;
            }

            Move move(square, toSquare, sliderPiece, side);

            move.capturedPiece = toPiece;

            moves.push_back(move);

            break;
        }
    }

    return moves;
}

std::vector<Move> Engine::getRookQuietMoves(int square, Piece sliderPiece, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 4; ++i) {
        int coefficient = 1;

        while (true) {
            int toRank = rank + coefficient * ROOK_MOVES[i][0];
            int toFile = file + coefficient * ROOK_MOVES[i][1];

            if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
                break;
            }

            int toSquare = this->_board.getSquare(toRank, toFile);

            if (this->_board.getPieceFromSquare(toRank, toFile) != Piece::EMPTY) {
                break;
            }

            moves.emplace_back(square, toSquare, sliderPiece, side);

            ++coefficient;
        }
    }

    return moves;
}

std::vector<Move> Engine::getRookCaptureMoves(int square, Piece sliderPiece, Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 4; ++i) {
        int coefficient = 1;

        while (true) {
            int toRank = rank + coefficient * ROOK_MOVES[i][0];
            int toFile = file + coefficient * ROOK_MOVES[i][1];

            if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
                break;
            }

            int toSquare = this->_board.getSquare(toRank, toFile);

            Piece toPiece = this->_board.getPieceFromSquare(toSquare);
            Colour toColour = this->_board.getColourFromSquare(toSquare);

            if (toPiece == Piece::EMPTY) {
                ++coefficient;

                continue;
            }

            if (toColour == side) {
                break;
            }

            Move move(square, toSquare, sliderPiece, side);

            move.capturedPiece = toPiece;

            moves.push_back(move);

            break;
        }
    }

    return moves;
}

std::vector<Move> Engine::getQueenQuietMoves(int square, Colour side) {
    std::vector<Move> moves;

    this->addToMoves(moves, this->getBishopQuietMoves(square, Piece::QUEEN, side));
    this->addToMoves(moves, this->getRookQuietMoves(square, Piece::QUEEN, side));

    return moves;
}

std::vector<Move> Engine::getQueenCaptureMoves(int square, Colour side) {
    std::vector<Move> moves;

    this->addToMoves(moves, this->getBishopCaptureMoves(square, Piece::QUEEN, side));
    this->addToMoves(moves, this->getRookCaptureMoves(square, Piece::QUEEN, side));

    return moves;
}

std::vector<Move> Engine::getKingQuietMoves(int square, engine::board::Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 8; ++i) {
        int toRank = rank + KING_MOVES[i][0];
        int toFile = file + KING_MOVES[i][1];

        if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
            continue;
        }

        int toSquare = this->_board.getSquare(toRank, toFile);

        if (this->_board.getPieceFromSquare(toSquare) != Piece::EMPTY) {
            continue;
        }

        moves.emplace_back(square, toSquare, Piece::KING, side);
    }

    return moves;
}

std::vector<Move> Engine::getKingCaptureMoves(int square, engine::board::Colour side) {
    std::vector<Move> moves;

    int rank = this->_board.getRankFromSquare(square);
    int file = this->_board.getFileFromSquare(square);

    for (int i = 0; i < 8; ++i) {
        int toRank = rank + KING_MOVES[i][0];
        int toFile = file + KING_MOVES[i][1];

        if (toRank < 0 || toRank >= 8 || toFile < 0 || toFile >= 8) {
            continue;
        }

        int toSquare = this->_board.getSquare(toRank, toFile);

        Piece toPiece = this->_board.getPieceFromSquare(toSquare);
        Colour toColour = this->_board.getColourFromSquare(toSquare);

        if (toPiece == Piece::EMPTY || toColour == side) {
            continue;
        }

        Move move(square, toSquare, Piece::KING, side);

        move.capturedPiece = toPiece;

        moves.push_back(move);
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

    // remember to change
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