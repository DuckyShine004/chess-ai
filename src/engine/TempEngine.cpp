#include <cstring>

#include "engine/TempEngine.hpp"

#include "engine/board/Castle.hpp"

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"
#include "utility/StringUtility.hpp"

using namespace engine::board;

using namespace engine::move;

using namespace utility;

namespace engine {

TempEngine::TempEngine() {
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

void TempEngine::printBoard() {
    BoardUtility::printBoard(this->_bitboards);
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

void TempEngine::createPiece(int rank, int file, engine::board::Piece piece, engine::board::Colour side) {
    this->createPiece(BoardUtility::getSquare(rank, file), piece, side);
}

void TempEngine::createPiece(int square, engine::board::Piece piece, engine::board::Colour side) {
    BitUtility::setBit(this->_bitboards[side][piece], square);

    BitUtility::setBit(this->_occupancies[side], square);

    this->_occupancyBoth = this->_occupancies[0] | this->_occupancies[1];
}

void TempEngine::generateMoves() {
}

void TempEngine::generatePawnMoves(std::vector<Move> &moves) {
}

void TempEngine::generateKnightMoves(std::vector<Move> &moves) {
}

void TempEngine::generateBishopMoves(std::vector<Move> &moves) {
}

void TempEngine::generateRookMoves(std::vector<Move> &moves) {
}

void TempEngine::generateQueenMoves(std::vector<Move> &moves) {
}

void TempEngine::generateKingMoves(std::vector<Move> &moves) {
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