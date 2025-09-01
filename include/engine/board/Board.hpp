#pragma once

#include <string>
#include <cctype>
#include <cstdint>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

namespace engine::board {

class Board {
  public:
    Board(); // Allow to pass in FEN later: TODO

    Board(std::string fen);

  private:
    uint8_t _squares[64];
    uint8_t _sideToMove;

    int8_t _enPassantSquare;

    uint16_t _halfMove;
    uint16_t _fullMove;

    int getSquare(int rank, int file);

    int getSquareFromPosition(const std::string &position);

    void createPiece(int rank, int file, Piece piece, Colour colour);

    void createPiece(int square, Piece piece, Colour colour);

    void createPieceFromFen(int rank, int file, char pieceNotation);
};

} // namespace engine::board