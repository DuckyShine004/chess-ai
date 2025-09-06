#include <cstring>

#include "engine/TempEngine.hpp"

#include "engine/board/Castle.hpp"
#include "engine/board/Square.hpp"

#include "engine/piece/Pawn.hpp"
#include "engine/piece/Knight.hpp"
#include "engine/piece/Bishop.hpp"
#include "engine/piece/Rook.hpp"
#include "engine/piece/Queen.hpp"
#include "engine/piece/King.hpp"

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"
#include "utility/StringUtility.hpp"

using namespace engine::board;

using namespace engine::piece;

using namespace engine::move;

using namespace utility;

namespace engine {

TempEngine::TempEngine() {
    this->initialise();

    this->parse(this->_INITIAL_FEN);
}

void TempEngine::parse(const char *fen) {
    this->reset();

    std::vector<std::string> states = StringUtility::splitStringByWhiteSpace(fen);

    this->parseFenPosition(states[0]);
    this->parseFenSide(states[1]);
    this->parseFenCastlingRights(states[2]);
    this->parseFenEnPassantSquare(states[3]);
    this->parseFenHalfMove(states[4]);
    this->parseFenFullMove(states[5]);
}

void TempEngine::run() {
    this->generateMoves(this->_side);
}

void TempEngine::printBoard() {
    BoardUtility::printBoard(this->_bitboards);
}

void TempEngine::initialise() {
    Bishop::initialiseRays();

    Rook::initialiseRays();

    for (int square = 0; square < 64; ++square) {
        this->_PAWN_ATTACKS[0][square] = Pawn::getAttacks(square, Colour::WHITE);
        this->_PAWN_ATTACKS[1][square] = Pawn::getAttacks(square, Colour::BLACK);

        this->_KNIGHT_ATTACKS[square] = Knight::getAttacks(square);

        this->_KING_ATTACKS[square] = King::getAttacks(square);
    }
}

void TempEngine::parseFenPosition(std::string &position) {
    int rank = 7;
    int file = 0;

    for (const char letter : position) {
        if (letter == '/') {
            file = 0;
            --rank;
        } else {
            if (StringUtility::isLetter(letter)) {
                this->createFenPiece(rank, file, letter);
                ++file;
            } else {
                file += letter - '0';
            }
        }
    }
}

void TempEngine::parseFenSide(std::string &side) {
    this->_side = (side == "w") ? Colour::WHITE : Colour::BLACK;
}

void TempEngine::parseFenCastlingRights(std::string &castlingRights) {
    this->_castlingRights = 0;

    if (castlingRights == "-") {
        return;
    }

    for (char c : castlingRights) {
        switch (c) {
        case 'K':
            this->_castlingRights |= Castle::WHITE_KING;
            break;
        case 'Q':
            this->_castlingRights |= Castle::WHITE_QUEEN;
            break;
        case 'k':
            this->_castlingRights |= Castle::BLACK_KING;
            break;
        case 'q':
            this->_castlingRights |= Castle::BLACK_QUEEN;
            break;
        default:
            break;
        }
    }
}

void TempEngine::parseFenEnPassantSquare(std::string &enPassantSquare) {
    this->_enPassantSquare = (enPassantSquare == "-") ? -1 : BoardUtility::getSquareFromPosition(enPassantSquare);
}

void TempEngine::parseFenHalfMove(std::string &halfMove) {
    this->_halfMove = (halfMove == "-") ? 1 : std::stoi(halfMove);
}

void TempEngine::parseFenFullMove(std::string &fullMove) {
    this->_fullMove = (fullMove == "-") ? 1 : std::stoi(fullMove);
}

void TempEngine::createFenPiece(int rank, int file, char letter) {
    Colour side = isupper(letter) ? Colour::WHITE : Colour::BLACK;
    Piece piece = Piece::EMPTY;

    letter = tolower(letter);

    switch (letter) {
    case 'p':
        piece = Piece::PAWN;
        break;
    case 'n':
        piece = Piece::KNIGHT;
        break;
    case 'b':
        piece = Piece::BISHOP;
        break;
    case 'r':
        piece = Piece::ROOK;
        break;
    case 'q':
        piece = Piece::QUEEN;
        break;
    case 'k':
        piece = Piece::KING;
        break;
    default:
        piece = Piece::EMPTY;
        break;
    }

    if (piece == Piece::EMPTY) {
        return;
    }

    this->createPiece(rank, file, piece, side);
}

void TempEngine::createPiece(int rank, int file, Piece piece, Colour side) {
    this->createPiece(BoardUtility::getSquare(rank, file), piece, side);
}

void TempEngine::createPiece(int square, Piece piece, Colour side) {
    BitUtility::setBit(this->_bitboards[side][piece], square);

    BitUtility::setBit(this->_occupancies[side], square);

    this->_occupancyBoth = this->_occupancies[0] | this->_occupancies[1];
}

void TempEngine::removePiece(int rank, int file, Piece piece, Colour side) {
    this->removePiece(BoardUtility::getSquare(rank, file), piece, side);
}

void TempEngine::removePiece(int square, Piece piece, Colour side) {
    BitUtility::clearBit(this->_bitboards[side][piece], square);

    BitUtility::clearBit(this->_occupancies[side], square);

    this->_occupancyBoth = this->_occupancies[0] | this->_occupancies[1];
}

// Sort moves by move type TODO
void TempEngine::generateMoves(Colour side) {
    std::vector<Move> moves;

    this->generatePawnMoves(moves, side);

    for (Move &move : moves) {
        makeMove(move);
        printBoard();
        unmakeMove(move);
    }

    this->printBoard();
}

void TempEngine::generatePawnMoves(std::vector<Move> &moves, Colour side) {
    Colour otherSide = BoardUtility::getOtherSide(side);

    uint64_t pawns = this->_bitboards[side][Piece::PAWN];

    uint64_t empty = ~this->_occupancyBoth;

    while (pawns) {
        int from = BitUtility::popLSB(pawns);

        if (Pawn::canSinglePush(from, side, empty)) {
            int to = Pawn::singlePush(from, side);

            moves.emplace_back(from, to, Piece::PAWN, side);

            // Pawn promotion TODO
        }

        if (Pawn::canDoublePush(from, side, empty)) {
            int to = Pawn::doublePush(from, side);

            Move move(from, to, Piece::PAWN, side);

            move.enPassantSquare = EN_PASSANT_SQUARES[side][BoardUtility::getFile(from)];

            moves.push_back(move);
        }

        uint64_t captureMoves = this->_PAWN_ATTACKS[side][from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, Piece::PAWN, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            moves.push_back(move);
        }

        // Check if we can capture enpassant square TODO
    }
}

void TempEngine::generateKnightMoves(std::vector<Move> &moves, Colour side) {
    Colour otherSide = BoardUtility::getOtherSide(side);

    uint64_t knights = this->_bitboards[side][Piece::KNIGHT];

    uint64_t empty = ~this->_occupancyBoth;

    while (knights) {
        int from = BitUtility::popLSB(knights);

        uint64_t quietMoves = this->_KNIGHT_ATTACKS[from] & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            moves.emplace_back(from, to, Piece::KNIGHT, side);
        }

        uint64_t captureMoves = this->_KNIGHT_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, Piece::KNIGHT, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            moves.push_back(move);
        }
    }
}

void TempEngine::generateBishopMoves(std::vector<Move> &moves, Colour side) {
}

void TempEngine::generateRookMoves(std::vector<Move> &moves, Colour side) {
}

void TempEngine::generateQueenMoves(std::vector<Move> &moves, Colour side) {
}

void TempEngine::generateKingMoves(std::vector<Move> &moves, Colour side) {
    Colour otherSide = BoardUtility::getOtherSide(side);

    uint64_t kings = this->_bitboards[side][Piece::KING];

    uint64_t empty = ~this->_occupancyBoth;

    while (kings) {
        int from = BitUtility::popLSB(kings);

        uint64_t quietMoves = this->_KING_ATTACKS[from] & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            moves.emplace_back(from, to, Piece::KING, side);
        }

        uint64_t captureMoves = this->_KING_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, Piece::KING, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            moves.push_back(move);
        }
    }
}

void TempEngine::makeMove(Move &move) {
    Colour otherSide = BoardUtility::getOtherSide(move.colour);

    this->removePiece(move.from, move.piece, move.colour);

    this->createPiece(move.to, move.piece, move.colour);

    if (move.capturedPiece != Piece::EMPTY) {
        this->removePiece(move.to, move.capturedPiece, otherSide);
    }
}

void TempEngine::unmakeMove(Move &move) {
    Colour otherSide = BoardUtility::getOtherSide(move.colour);

    this->removePiece(move.to, move.piece, move.colour);

    this->createPiece(move.from, move.piece, move.colour);

    if (move.capturedPiece != Piece::EMPTY) {
        this->createPiece(move.to, move.capturedPiece, otherSide);
    }
}

void TempEngine::reset() {
    memset(this->_bitboards, 0ULL, sizeof(this->_bitboards));

    memset(this->_occupancies, 0ULL, sizeof(this->_occupancies));

    this->_occupancyBoth = 0ULL;
    this->_castlingRights = this->_INITIAL_CASTLING_RIGHTS;
    this->_side = Colour::WHITE;
    this->_enPassantSquare = -1;
}

} // namespace engine