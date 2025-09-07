#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <climits>

#include "engine/board/Colour.hpp"

#include "engine/move/Move.hpp"

namespace engine {

struct SearchResult {
    int bestScore;

    engine::move::Move bestMove;

    bool isMoveFound;

    SearchResult() : bestScore(-INT_MAX), isMoveFound(false) {
    }
};

class Engine {
  public:
    Engine();

    void parse(const char *fen);

    void run();

    void switchSide();

    int getPly();

    void runPerft(int depth);

    uint64_t getBoard();

    void printBoard();

  private:
    static inline constexpr const char *_INITIAL_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    static inline constexpr uint8_t _INITIAL_CASTLING_RIGHTS = 0xF;

    static inline constexpr int _SEARCH_DEPTH = 5;

    static inline uint64_t _PAWN_ATTACKS[2][64];
    static inline uint64_t _KNIGHT_ATTACKS[64];
    static inline uint64_t _KING_ATTACKS[64];

    uint64_t _bitboards[2][6];
    uint64_t _occupancies[2];
    uint64_t _occupancyBoth;

    uint8_t _castlingRights;

    uint16_t _halfMove;
    uint16_t _fullMove;

    engine::board::Colour _side;

    int _enPassantSquare;
    int _ply;

    void initialise();

    void parseFenPosition(std::string &position);

    void parseFenSide(std::string &side);

    void parseFenCastlingRights(std::string &castlingRights);

    void parseFenEnPassantSquare(std::string &enPassantSquare);

    void parseFenHalfMove(std::string &halfMove);

    void parseFenFullMove(std::string &fullMove);

    void createFenPiece(int rank, int file, char letter);

    void createPiece(int rank, int file, engine::board::Piece piece, engine::board::Colour side);

    void createPiece(int square, engine::board::Piece piece, engine::board::Colour side);

    void removePiece(int rank, int file, engine::board::Piece piece, engine::board::Colour side);

    void removePiece(int square, engine::board::Piece piece, engine::board::Colour side);

    std::vector<engine::move::Move> generateMoves(engine::board::Colour side);

    void generatePawnMoves(std::vector<engine::move::Move> &moves, engine::board::Colour side);

    void generateKnightMoves(std::vector<engine::move::Move> &moves, engine::board::Colour side);

    void generateBishopMoves(std::vector<engine::move::Move> &moves, engine::board::Colour side);

    void generateRookMoves(std::vector<engine::move::Move> &moves, engine::board::Colour side);

    void generateQueenMoves(std::vector<engine::move::Move> &moves, engine::board::Colour side);

    void generateKingMoves(std::vector<engine::move::Move> &moves, engine::board::Colour side);

    std::vector<engine::move::Move> generateCaptures(engine::board::Colour side);

    void generatePawnCaptures(std::vector<engine::move::Move> &captures, engine::board::Colour side);

    void generateKnightCaptures(std::vector<engine::move::Move> &captures, engine::board::Colour side);

    void generateBishopCaptures(std::vector<engine::move::Move> &captures, engine::board::Colour side);

    void generateRookCaptures(std::vector<engine::move::Move> &captures, engine::board::Colour side);

    void generateQueenCaptures(std::vector<engine::move::Move> &captures, engine::board::Colour side);

    void generateKingCaptures(std::vector<engine::move::Move> &captures, engine::board::Colour side);

    bool isMoveLegal(engine::move::Move &move, engine::board::Colour side);

    bool isInCheck(engine::board::Colour side);

    bool isSquareAttacked(int square, engine::board::Colour side);

    int getKingSquare(engine::board::Colour side);

    void makeMove(engine::move::Move &move);

    void unmakeMove(engine::move::Move &move);

    SearchResult searchRoot(int depth);

    int search(int alpha, int beta, int depth);

    int quiescence(int alpha, int beta);

    int evaluate(engine::board::Colour side);

    int perft(int depth);

    void reset();
};

} // namespace engine