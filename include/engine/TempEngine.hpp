#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "engine/board/Colour.hpp"

#include "engine/move/Move.hpp"

namespace engine {

class TempEngine {
  public:
    TempEngine();

    void parse(const char *fen);

    void printBoard();

  private:
    static inline constexpr const char *_INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    static inline constexpr uint8_t _INITIAL_CASTLING_RIGHTS = 0xF;

    uint64_t _bitboards[2][6];
    uint64_t _occupancies[2];
    uint64_t _occupancyBoth;

    uint8_t _castlingRights;

    uint16_t _halfMove;
    uint16_t _fullMove;

    engine::board::Colour _side;

    int _enPassantSquare;

    void parseFenPosition(std::string &position);

    void parseFenSide(std::string &side);

    void parseFenCastlingRights(std::string &castlingRights);

    void parseFenEnPassantSquare(std::string &enPassantSquare);

    void parseFenHalfMove(std::string &halfMove);

    void parseFenFullMove(std::string &fullMove);

    void createFenPiece(int rank, int file, char letter);

    void createPiece(int rank, int file, engine::board::Piece piece, engine::board::Colour side);

    void createPiece(int square, engine::board::Piece piece, engine::board::Colour side);

    void generateMoves();

    void generatePawnMoves(std::vector<engine::move::Move> &moves);

    void generateKnightMoves(std::vector<engine::move::Move> &moves);

    void generateBishopMoves(std::vector<engine::move::Move> &moves);

    void generateRookMoves(std::vector<engine::move::Move> &moves);

    void generateQueenMoves(std::vector<engine::move::Move> &moves);

    void generateKingMoves(std::vector<engine::move::Move> &moves);

    void reset();
};

} // namespace engine