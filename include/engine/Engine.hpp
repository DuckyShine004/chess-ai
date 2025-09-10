#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <climits>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

#include "engine/move/Move.hpp"
#include "engine/move/Undo.hpp"

namespace engine {

struct SearchResult {
    int bestScore;

    engine::move::Move bestMove;

    bool isMoveFound;

    SearchResult() : bestScore(-INT_MAX), isMoveFound(false) {
    }

    void clear() {
        this->bestScore = -INT_MAX;

        this->isMoveFound = false;
    }
};

class Engine {
  public:
    Engine();

    void parse(const char *fen);

    void run();

    engine::move::Move &getMove();

    void switchSide();

    int getPly();

    engine::board::PieceType getPiece(int square, engine::board::ColourType side);

    std::vector<engine::move::Move> generateMoves(engine::board::ColourType side);

    void makeMove(engine::move::Move &move);

    void runPerft(int depth);

    engine::board::ColourType getSide();

    void printBoard();

  private:
    static inline constexpr const char *_RANDOM_FEN = "3B1Qr1/P2p1bP1/1P1p1pk1/pNnP1ppR/P1PPpR2/pBq2P2/1r1N1n2/2b2K2 w - - 0 1";

    static inline constexpr uint8_t _INITIAL_CASTLE_RIGHTS = 0xF;

    static inline constexpr int _SEARCH_DEPTH = 1;

    static inline uint64_t _PAWN_ATTACKS[2][64];
    static inline uint64_t _KNIGHT_ATTACKS[64];
    static inline uint64_t _KING_ATTACKS[64];

    uint64_t _bitboards[2][6];
    uint64_t _occupancies[2];
    uint64_t _occupancyBoth;

    uint8_t _castleRights;

    uint16_t _halfMove;
    uint16_t _fullMove;

    engine::board::ColourType _side;

    SearchResult _searchResult;

    int _enPassantSquare;
    int _ply;

    std::vector<engine::move::Undo> _undoStack;

    void initialise();

    void parseFenPosition(std::string &position);

    void parseFenSide(std::string &side);

    void parseFenCastleRights(std::string &castleRights);

    void parseFenEnPassantSquare(std::string &enPassantSquare);

    void parseFenHalfMove(std::string &halfMove);

    void parseFenFullMove(std::string &fullMove);

    void createFenPiece(int rank, int file, char letter);

    void createPiece(int rank, int file, engine::board::ColourType side);

    void createPiece(int square, engine::board::ColourType side);

    void createPiece(int rank, int file, engine::board::PieceType piece, engine::board::ColourType side);

    void createPiece(int square, engine::board::PieceType piece, engine::board::ColourType side);

    void removePiece(int rank, int file, engine::board::ColourType side);

    void removePiece(int square, engine::board::ColourType side);

    void removePiece(int rank, int file, engine::board::PieceType piece, engine::board::ColourType side);

    void removePiece(int square, engine::board::PieceType piece, engine::board::ColourType side);

    void generatePawnMoves(std::vector<engine::move::Move> &moves, engine::board::ColourType side);

    void generateKnightMoves(std::vector<engine::move::Move> &moves, engine::board::ColourType side);

    void generateBishopMoves(std::vector<engine::move::Move> &moves, engine::board::ColourType side);

    void generateRookMoves(std::vector<engine::move::Move> &moves, engine::board::ColourType side);

    void generateQueenMoves(std::vector<engine::move::Move> &moves, engine::board::ColourType side);

    void generateKingMoves(std::vector<engine::move::Move> &moves, engine::board::ColourType side);

    void generateCastleMoves(std::vector<engine::move::Move> &moves, engine::board::ColourType side);

    std::vector<engine::move::Move> generateCaptures(engine::board::ColourType side);

    void generatePawnCaptures(std::vector<engine::move::Move> &captures, engine::board::ColourType side);

    void generateKnightCaptures(std::vector<engine::move::Move> &captures, engine::board::ColourType side);

    void generateBishopCaptures(std::vector<engine::move::Move> &captures, engine::board::ColourType side);

    void generateRookCaptures(std::vector<engine::move::Move> &captures, engine::board::ColourType side);

    void generateQueenCaptures(std::vector<engine::move::Move> &captures, engine::board::ColourType side);

    void generateKingCaptures(std::vector<engine::move::Move> &captures, engine::board::ColourType side);

    bool isMoveLegal(engine::move::Move &move, engine::board::ColourType side);

    bool isInCheck(engine::board::ColourType side);

    bool isSquareAttacked(int square, engine::board::ColourType side);

    bool areSquaresAttacked(uint64_t squares, engine::board::ColourType side);

    int getKingSquare(engine::board::ColourType side);

    void updateCastleRights();

    void updateCastleRights(engine::board::ColourType side);

    void unmakeMove(engine::move::Move &move);

    void searchRoot(int depth);

    int search(int alpha, int beta, int depth);

    int quiescence(int alpha, int beta);

    int evaluate(engine::board::ColourType side);

    int perft(int depth);

    void reset();
};

} // namespace engine
