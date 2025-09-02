#pragma once

#include <string>
#include <cctype>
#include <cstdint>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace engine::board {

class Board {
  public:
    Board();

    Board(std::string fen);

    bool isWhiteTurn();

    Piece getPieceFromSquare(int rank, int file);

    Piece getPieceFromSquare(int square);

    Colour getColourFromSquare(int rank, int file);

    Colour getColourFromSquare(int square);

    void createPieceInSquare(int rank, int file, Piece piece, Colour colour);

    void createPieceInSquare(int square, Piece piece, Colour colour);

    void removeFromSquare(int rank, int file);

    void removeFromSquare(int square);

    int getRankFromSquare(int square);

    int getFileFromSquare(int square);

    int getSquareFromPosition(const std::string &position);

    void print();

  private:
    static constexpr uint8_t _PIECE_MASK = 0x07;
    static constexpr uint8_t _COLOUR_MASK = 0x08;
    static constexpr uint8_t _COLOUR_BIT = 3;
    static constexpr uint8_t _FULL_CASTLE_MASK = 0xf;

    uint8_t _squares[64];

    uint8_t _sideToMove;
    uint8_t _castlingRights;

    int8_t _enPassantSquare;

    uint16_t _halfMove;
    uint16_t _fullMove;

    int getSquare(int rank, int file);

    void createPieceFromFen(int rank, int file, char pieceNotation);
};

} // namespace engine::board