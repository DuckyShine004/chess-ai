#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <climits>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

#include "engine/move/Move.hpp"
#include "engine/move/Undo.hpp"

#include "compiler/compiler.hpp"

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

    engine::move::MoveList generateMoves(engine::board::ColourType side);

    void makeMove(engine::move::Move &move);

    void runPerft(int depth);

    engine::board::ColourType getSide();

    void printBoard();

  private:
    static inline constexpr uint8_t _INITIAL_CASTLE_RIGHTS = 0xF;

    static inline constexpr int _SEARCH_DEPTH = 5;

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

    void generatePawnMoves(engine::move::MoveList &moves, engine::board::ColourType side);

    void generateKnightMoves(engine::move::MoveList &moves, engine::board::ColourType side);

    void generateBishopMoves(engine::move::MoveList &moves, engine::board::ColourType side);

    void generateRookMoves(engine::move::MoveList &moves, engine::board::ColourType side);

    void generateQueenMoves(engine::move::MoveList &moves, engine::board::ColourType side);

    void generateKingMoves(engine::move::MoveList &moves, engine::board::ColourType side);

    void generateCastleMoves(engine::move::MoveList &moves, engine::board::ColourType side);

    engine::move::MoveList generateCaptures(engine::board::ColourType side);

    void generatePawnCaptures(engine::move::MoveList &captures, engine::board::ColourType side);

    void generateKnightCaptures(engine::move::MoveList &captures, engine::board::ColourType side);

    void generateBishopCaptures(engine::move::MoveList &captures, engine::board::ColourType side);

    void generateRookCaptures(engine::move::MoveList &captures, engine::board::ColourType side);

    void generateQueenCaptures(engine::move::MoveList &captures, engine::board::ColourType side);

    void generateKingCaptures(engine::move::MoveList &captures, engine::board::ColourType side);

    bool isMoveLegal(engine::move::Move &move, engine::board::ColourType side);

    bool isInCheck(engine::board::ColourType side);

    bool isSquareAttacked(int square, engine::board::ColourType side);

    bool areSquaresAttacked(uint64_t squares, engine::board::ColourType side);

    int getKingSquare(engine::board::ColourType side);

    void updateCastleRights();

    void updateCastleRights(engine::board::ColourType side);

    void unmakeMove(engine::move::Move &move);

    FORCE_INLINE void makeQuietMove(const engine::move::Move &move, engine::board::PieceType fromPiece);

    FORCE_INLINE void makeCaptureMove(const engine::move::Move &move, engine::move::Undo &undo, engine::board::PieceType fromPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void makeDoublePawnMove(const engine::move::Move &move, engine::board::PieceType fromPiece);

    FORCE_INLINE void makeEnPassantMove(const engine::move::Move &move, engine::board::PieceType fromPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void makeQueenCastleMove(const engine::move::Move &move, engine::board::PieceType fromPiece);

    FORCE_INLINE void makeKingCastleMove(const engine::move::Move &move, engine::board::PieceType fromPiece);

    FORCE_INLINE void makePromotionQuietMove(const engine::move::Move &move);

    FORCE_INLINE void makePromotionCaptureMove(const engine::move::Move &move, engine::move::Undo &undo, engine::board::ColourType otherSide);

    FORCE_INLINE void unmakeQuietMove(const engine::move::Move &move, engine::board::PieceType toPiece);

    FORCE_INLINE void unmakeCaptureMove(const engine::move::Move &move, const engine::move::Undo &undo, engine::board::PieceType toPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void unmakeEnPassantMove(const engine::move::Move &move, engine::board::PieceType toPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void unmakeQueenCastleMove(const engine::move::Move &move, engine::board::PieceType toPiece);

    FORCE_INLINE void unmakeKingCastleMove(const engine::move::Move &move, engine::board::PieceType toPiece);

    FORCE_INLINE void unmakePromotionQuietMove(const engine::move::Move &move);

    FORCE_INLINE void unmakePromotionCaptureMove(const engine::move::Move &move, const engine::move::Undo &undo, engine::board::ColourType otherSide);

    void searchRoot(int depth);

    int search(int alpha, int beta, int depth);

    int quiescence(int alpha, int beta);

    int evaluate(engine::board::ColourType side);

    int perft(int depth);

    void reset();
};

} // namespace engine
