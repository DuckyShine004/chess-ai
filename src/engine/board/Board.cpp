#include <cstring>

#include "engine/board/Board.hpp"
#include "engine/board/Piece.hpp"

#include "utility/StringUtility.hpp"

using namespace engine::board;

using namespace utility;

namespace engine::board {

Board::Board() {
    memset(this->_squares, 0, sizeof(this->_squares));

    Piece backRankPieces[8] = {
        Piece::ROOK, Piece::KNIGHT, Piece::BISHOP, Piece::QUEEN, Piece::KING, Piece::BISHOP, Piece::KNIGHT, Piece::ROOK,
    };

    for (int file = 0; file < 8; ++file) {
        this->createPiece(this->getSquare(0, file), backRankPieces[file], Colour::WHITE);
        this->createPiece(this->getSquare(1, file), Piece::PAWN, Colour::WHITE);
    }

    for (int file = 0; file < 8; ++file) {
        this->createPiece(this->getSquare(7, file), backRankPieces[file], Colour::BLACK);
        this->createPiece(this->getSquare(6, file), Piece::PAWN, Colour::BLACK);
    }

    this->_sideToMove = Colour::WHITE;
    // this->_castlingFlags=
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
                createPieceFromFen(rank, file, letter);
                ++file;
            } else {
                file += letter - '0';
            }
        }
    }

    this->_sideToMove = fenStates[1] == "w" ? Colour::WHITE : Colour::BLACK;

    // castling rights third field TODO

    this->_enPassantSquare = fenStates[3] == "-" ? -1 : getSquareFromPosition(fenStates[3]);

    this->_halfMove = std::stoi(fenStates[4]);
    this->_fullMove = std::stoi(fenStates[5]);
}

int Board::getSquare(int rank, int file) {
    return rank * 8 + file;
}

int Board::getSquareFromPosition(const std::string &position) {
    int file = position[0] - 'a';
    int rank = position[1] - '1';

    return this->getSquare(rank, file);
}

void Board::createPiece(int rank, int file, Piece piece, Colour colour) {
    int square = this->getSquare(rank, file);

    this->createPiece(square, piece, colour);
}

void Board::createPiece(int square, Piece piece, Colour colour) {
    this->_squares[square] = (piece | (colour << 3));
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

    createPiece(rank, file, piece, colour);
}

} // namespace engine::board