#include <cstring>
#include <iostream>

#include "engine/board/Board.hpp"
#include "engine/board/Piece.hpp"
#include "engine/board/Castle.hpp"

#include "utility/StringUtility.hpp"

using namespace engine::board;

using namespace utility;

namespace engine::board {

Board::Board() {
    memset(this->_squares, 0, sizeof(this->_squares));

    for (int file = 0; file < 8; ++file) {
        this->createPieceInSquare(this->getSquare(0, file), BACKRANK_PIECES[file], Colour::WHITE);
        this->createPieceInSquare(this->getSquare(1, file), Piece::PAWN, Colour::WHITE);
    }

    for (int file = 0; file < 8; ++file) {
        this->createPieceInSquare(this->getSquare(7, file), BACKRANK_PIECES[file], Colour::BLACK);
        this->createPieceInSquare(this->getSquare(6, file), Piece::PAWN, Colour::BLACK);
    }

    this->_sideToMove = Colour::WHITE;
    this->_castlingRights = this->_FULL_CASTLE_MASK;
    this->_enPassantSquare = -1;
    this->_halfMove = 0;
    this->_fullMove = 1;
}

Board::Board(std::string fen) {
    memset(this->_squares, 0, sizeof(this->_squares));

    std::vector<std::string> fenStates = StringUtility::splitStringByWhiteSpace(fen);

    if (fenStates.size() != 6) {
        return;
    }

    int rank = 7;
    int file = 0;

    for (char letter : fenStates[0]) {
        if (letter == '/') {
            file = 0;
            --rank;
        } else {
            if (StringUtility::isLetter(letter)) {
                this->createPieceFromFen(rank, file, letter);
                ++file;
            } else {
                file += letter - '0';
            }
        }
    }

    this->_sideToMove = fenStates[1] == "w" ? Colour::WHITE : Colour::BLACK;

    this->_castlingRights = 0;
    std::string castlingRights = fenStates[2];

    if (fenStates[2] != "-") {
        for (char c : fenStates[2]) {
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

    this->_enPassantSquare = fenStates[3] == "-" ? -1 : this->getSquareFromPosition(fenStates[3]);

    this->_halfMove = fenStates[4] == "-" ? 0 : std::stoi(fenStates[4]);
    this->_fullMove = fenStates[5] == "-" ? 1 : std::stoi(fenStates[5]);
}

int Board::getSquare(int rank, int file) {
    return rank * 8 + file;
}

int Board::getSquareFromPosition(const std::string &position) {
    int file = position[0] - 'a';
    int rank = position[1] - '1';

    return this->getSquare(rank, file);
}

Piece Board::getPieceFromSquare(int rank, int file) {
    int square = this->getSquare(rank, file);

    return this->getPieceFromSquare(square);
}

Piece Board::getPieceFromSquare(int square) {
    uint8_t encoding = this->_squares[square];

    uint8_t piece = encoding & this->_PIECE_MASK;

    return static_cast<Piece>(piece);
}

Colour Board::getColourFromSquare(int rank, int file) {
    int square = this->getSquare(rank, file);

    return this->getColourFromSquare(square);
}

Colour Board::getColourFromSquare(int square) {
    uint8_t encoding = this->_squares[square];

    uint8_t colour = (encoding & this->_COLOUR_MASK) >> this->_COLOUR_BIT;

    return static_cast<Colour>(colour);
}

void Board::createPieceInSquare(int rank, int file, Piece piece, Colour colour) {
    int square = this->getSquare(rank, file);

    this->createPieceInSquare(square, piece, colour);
}

void Board::createPieceInSquare(int square, Piece piece, Colour colour) {
    this->_squares[square] = (piece | (colour << this->_COLOUR_BIT));
}

void Board::removeFromSquare(int rank, int file) {
    int square = this->getSquare(rank, file);

    this->removeFromSquare(square);
}

void Board::removeFromSquare(int square) {
    this->_squares[square] = Piece::EMPTY;
}

int Board::getRankFromSquare(int square) {
    return square / 8;
}

int Board::getFileFromSquare(int square) {
    return square % 8;
}

void Board::createPieceFromFen(int rank, int file, char pieceNotation) {
    Colour colour = isupper(pieceNotation) ? Colour::WHITE : Colour::BLACK;

    Piece piece = Piece::EMPTY;

    pieceNotation = tolower(pieceNotation);

    switch (pieceNotation) {
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

    this->createPieceInSquare(rank, file, piece, colour);
}

bool Board::isWhiteTurn() {
    return this->_sideToMove == Colour::WHITE;
}

void Board::print() {
    std::cout << "  +---+---+---+---+---+---+---+---+\n";

    for (int rank = 7; rank >= 0; --rank) {
        std::cout << std::to_string(rank + 1) << " |";

        for (int file = 0; file < 8; ++file) {
            Piece piece = this->getPieceFromSquare(rank, file);

            if (piece == Piece::EMPTY) {
                std::cout << "   |";
            } else {
                Colour colour = this->getColourFromSquare(rank, file);

                std::string unicodePiece = UNICODE_PIECES[(piece - 1) + 6 * colour];

                std::cout << ' ' << unicodePiece << " |";
            }
        }

        std::cout << "\n  +---+---+---+---+---+---+---+---+\n";
    }

    std::cout << "    A   B   C   D   E   F   G   H\n";
}

} // namespace engine::board