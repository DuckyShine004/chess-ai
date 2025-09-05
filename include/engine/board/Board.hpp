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

    Colour getSide();

    void switchSide();

    void createPieceInSquare(int rank, int file, Piece piece, Colour colour);

    void createPieceInSquare(int square, Piece piece, Colour colour);

    void removeFromSquare(int rank, int file);

    void removeFromSquare(int square);

    int getSquare(int rank, int file);

    int getRankFromSquare(int square);

    int getFileFromSquare(int square);

    int getSquareFromPosition(const std::string &position);

    std::string getPositionFromSquare(int square);

    int getMirroredSquare(int square);

    void incrementHalfMove();

    void decrementHalfMove();

    void resetHalfMove();

    void incrementFullMove();

    void decrementFullMove();

    int getHalfMove();

    void print();

  private:
    static constexpr uint8_t _PIECE_MASK = 0x07;
    static constexpr uint8_t _COLOUR_MASK = 0x08;
    static constexpr uint8_t _COLOUR_BIT = 3;
    static constexpr uint8_t _FULL_CASTLE_MASK = 0xf;

    uint8_t _squares[64];

    Colour _side;

    uint8_t _castlingRights;

    int8_t _enPassantSquare;

    uint16_t _halfMove;
    uint16_t _fullMove;

    void createPieceFromFen(int rank, int file, char pieceNotation);
};

} // namespace engine::board