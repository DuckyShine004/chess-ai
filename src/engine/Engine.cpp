#include <iostream>
#include <algorithm>

#include "engine/Engine.hpp"

#include "engine/evaluation/Material.hpp"
#include "engine/evaluation/Position.hpp"

#include "engine/piece/Pawn.hpp"
#include "engine/piece/Knight.hpp"
#include "engine/piece/King.hpp"

#include "utility/BoardUtility.hpp"
#include "utility/BitUtility.hpp"

using namespace engine::board;

using namespace engine::piece;

using namespace engine::move;

using namespace engine::evaluation;

using namespace utility;

namespace engine {

Engine::Engine() : _zobrist(0ULL) {
    this->initialise_attack_tables();
    std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    // std::string fen = "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2";
    // std::string fen = "rnbqkbnr/pppppppp/8/8/2N5/1P1K4/P1PPPPPP/R1BQ1BNR w KQkq - 0 1";
    // std::string fen = "8/8/2b5/8/1N2K3/8/8/8 w - - 0 1";

    this->_board = Board(fen);

    this->_ply = 0;

    this->_key = this->_zobrist.hash(this->_board);

    this->_keyBuffer.push_back(this->_key);
}

void Engine::initialise_attack_tables() {
    for (int square = 0; square < 64; ++square) {
        int rank = BoardUtility::getRank(square);
        int file = BoardUtility::getFile(square);

        this->_PAWN_ATTACK_TABLE[0][square] = Pawn::getAttacks(square, Colour::WHITE);
        this->_PAWN_ATTACK_TABLE[1][square] = Pawn::getAttacks(square, Colour::BLACK);

        this->_KNIGHT_ATTACK_TABLE[square] = Knight::getAttacks(square);

        this->_KING_ATTACK_TABLE[square] = King::getAttacks(square);

        BitUtility::printBitBoard(this->_KING_ATTACK_TABLE[square]);
    }
}

void Engine::run() {

    // for (int i = 0; i < 50; ++i) {
    //     Result result = this->searchRoot(3);

    //     if (result.isMoveFound) {
    //         this->makeMove(result.bestMove);

    //         this->_board.print();

    //         std::cout << "Score: " << result.bestScore << '\n';
    //     }
    // }
}

Board &Engine::getBoard() {
    return this->_board;
}

std::vector<Move> Engine::getMoves(Colour side) {
    std::vector<Move> moves;

    this->addToMoves(moves, this->getCaptureMoves(side));
    this->addToMoves(moves, this->getQuietMoves(side));

    return moves;
}

std::vector<Move> Engine::getLegalMoves(Colour side) {
    std::vector<Move> moves = this->getMoves(side);

    return moves;
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

bool Engine::isMoveLegal(Move &move) {
    bool isMoveLegal = true;

    this->makeMove(move);

    Colour otherSide = static_cast<Colour>(move.colour ^ 1);

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
    this->_keyBuffer.push_back(this->_key);

    this->_board.removeFromSquare(move.from);
    this->_key ^= this->_zobrist.getKey().piece[move.from][BoardUtility::getPieceIndex(move.piece, move.colour)];

    this->_board.createPieceInSquare(move.to, move.piece, move.colour);
    this->_key ^= this->_zobrist.getKey().piece[move.to][BoardUtility::getPieceIndex(move.piece, move.colour)];

    if (move.capturedPiece != Piece::EMPTY) {
        Colour otherColour = static_cast<Colour>(move.colour ^ 1);

        this->_key ^= this->_zobrist.getKey().piece[move.to][BoardUtility::getPieceIndex(move.capturedPiece, otherColour)];
    }

    if (move.piece == Piece::PAWN || move.capturedPiece != Piece::EMPTY) {
        this->_board.resetHalfMove();
    } else {
        this->_board.incrementHalfMove();
    }

    if (move.colour == Colour::BLACK) {
        this->_board.incrementFullMove();
    }

    this->_board.switchSide();
    this->_key ^= this->_zobrist.getKey().side;

    ++this->_ply;
}

void Engine::unmakeMove(Move &move) {
    this->_board.switchSide();
    this->_key ^= this->_zobrist.getKey().side;

    this->_board.removeFromSquare(move.to);
    this->_key ^= this->_zobrist.getKey().piece[move.to][BoardUtility::getPieceIndex(move.piece, move.colour)];

    this->_board.createPieceInSquare(move.from, move.piece, move.colour);
    this->_key ^= this->_zobrist.getKey().piece[move.from][BoardUtility::getPieceIndex(move.piece, move.colour)];

    // If there was a captured piece, then move the captured piece back
    if (move.capturedPiece != Piece::EMPTY) {
        Colour otherColour = static_cast<Colour>(move.colour ^ 1);

        this->_board.createPieceInSquare(move.to, move.capturedPiece, otherColour);

        this->_key ^= this->_zobrist.getKey().piece[move.to][BoardUtility::getPieceIndex(move.capturedPiece, otherColour)];
    }

    this->_key = this->_keyBuffer.back();

    this->_keyBuffer.pop_back();

    --this->_ply;
}

Result Engine::searchRoot(int depth) {
    Result result;

    int alpha = -INT_MAX;
    int beta = INT_MAX;

    std::vector<Move> moves = this->getMoves(this->_board.getSide());

    for (Move &move : moves) {
        if (!this->isMoveLegal(move)) {
            continue;
        }

        this->makeMove(move);

        int score = -this->search(-beta, -alpha, depth - 1);

        this->unmakeMove(move);

        if (score > result.bestScore) {
            result.bestScore = score;

            result.bestMove = move;

            result.isMoveFound = true;

            if (score > alpha) {
                alpha = score;
            }
        }

        // beta cutoff
        if (score >= beta) {
            break;
        }
    }

    return result;
}

// Implement null heuristic and mobility score TODO
int Engine::search(int alpha, int beta, int depth) {
    if (this->isRepetition()) {
        return DRAW_VALUE;
    }

    if (depth == 0) {
        return this->quiescence(alpha, beta);
    }

    int bestScore = -INT_MAX;

    std::vector<Move> moves = this->getMoves(this->_board.getSide());

    for (Move &move : moves) {
        if (!this->isMoveLegal(move)) {
            continue;
        }

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
    if (this->isRepetition()) {
        return DRAW_VALUE;
    }

    Colour side = this->_board.getSide();

    int bestScore = this->evaluate(side);

    if (bestScore >= beta) {
        return bestScore;
    }

    if (bestScore > alpha) {
        alpha = bestScore;
    }

    std::vector<Move> captures = this->getCaptureMoves(side);

    for (Move &capture : captures) {
        if (!this->isMoveLegal(capture)) {
            continue;
        }

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

int Engine::evaluate(Colour side) {
    int score = 0;

    for (int square = 0; square < 64; ++square) {
        Piece piece = this->_board.getPieceFromSquare(square);

        if (piece == Piece::EMPTY) {
            continue;
        }

        Colour colour = this->_board.getColourFromSquare(square);

        int sign = (colour == Colour::WHITE) ? 1 : -1;

        score += sign * MATERIAL_TABLE[piece - 1];

        int trueSquare = (colour == Colour::WHITE) ? square : this->_board.getMirroredSquare(square);

        switch (piece) {
        case Piece::PAWN:
            score += sign * PAWN_POSITION_TABLE[trueSquare];
            break;
        case Piece::KNIGHT:
            score += sign * KNIGHT_POSITION_TABLE[trueSquare];
            break;
        case Piece::BISHOP:
            score += sign * BISHOP_POSITION_TABLE[trueSquare];
            break;
        case Piece::ROOK:
            score += sign * ROOK_POSITION_TABLE[trueSquare];
            break;
        case Piece::QUEEN:
            score += sign * QUEEN_POSITION_TABLE[trueSquare];
            break;
        default:
            break;
        }
    }

    int mobilityScore = this->getMoves(side).size() - this->getMoves(static_cast<Colour>(side ^ 1)).size();

    score += mobilityScore;
    return (side == Colour::WHITE) ? score : -score;
}

bool Engine::isRepetition() {
    // halfmove and full move are permanent once undo is implemented, uncomment
    // if (this->_board.getHalfMove() < 4) {
    //     return false;
    // }

    // for (int i = 0; i < this->_keyBuffer.size(); ++i) {
    //     // std::cout << this->_keyBuffer[i] << '\n';
    //     if (this->_keyBuffer[i] == this->_key) {
    //         return true;
    //     }
    // }

    return false;
}

} // namespace engine