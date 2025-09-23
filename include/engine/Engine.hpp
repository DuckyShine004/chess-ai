#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "engine/board/Piece.hpp"
#include "engine/board/Colour.hpp"

#include "engine/move/Move.hpp"
#include "engine/move/Undo.hpp"
#include "engine/move/Order.hpp"

#include "engine/hash/Transposition.hpp"

#include "engine/evaluation/Score.hpp"

#include "compiler/compiler.hpp"

namespace engine {

struct SearchResult {
    int bestScore;

    uint16_t bestMove;

    int nodes;

    SearchResult() : bestScore(-engine::evaluation::Score::INF), nodes(0) {
    }

    void clear() {
        this->bestScore = -engine::evaluation::Score::INF;

        this->nodes = 0;
    }
};

class Engine {
  public:
    Engine();

    void parse(const char *fen);

    void run();

    uint16_t &getMove();

    void switchSide();

    engine::board::PieceType getPiece(int square, engine::board::ColourType side);

    engine::move::Move::MoveList generateMoves(engine::board::ColourType side);

    void makeMove(uint16_t &move);

    bool isInCheck();

    bool isMoveLegal(uint16_t &move, engine::board::ColourType side);

    void runPerft(int depth);

    engine::board::ColourType getSide();

    void printBoard();

  private:
    static inline constexpr engine::board::ColourType _INITIAL_SIDE = engine::board::ColourType::WHITE;

    static inline constexpr uint8_t _INITIAL_CASTLE_RIGHTS = 0xF;

    static inline constexpr int _MAX_HALF_MOVES = 50;

    // LMR
    static inline constexpr int _FULL_DEPTH = 4;
    static inline constexpr int _REDUCTION_DEPTH = 2;
    static inline constexpr int _REDUCTION_LIMIT = 3;

    // NMP
    static inline constexpr int _NMP_REDUCTION = 2;
    static inline constexpr bool _DO_NMP = true;
    static inline constexpr bool _DO_NOT_NMP = false;

    static inline constexpr int _SEARCH_DEPTH = 9;

    // Repetition table size
    static inline constexpr int _REPETITION_TABLE_SIZE = 1024;

    // Aspiration Window
    static inline constexpr int _ASPIRATION_WINDOW_VALUE = 50;

    engine::hash::Transposition::Entry _transpositionTable[engine::hash::Transposition::TRANSPOSITION_TABLE_ENTRIES];

    uint64_t _bitboards[2][6];
    uint64_t _occupancies[2];
    uint64_t _occupancyBoth;
    int _material_scores[2];

    uint64_t _zobrist;

    uint8_t _castleRights;

    uint16_t _halfMove;
    uint16_t _fullMove;

    engine::board::ColourType _side;

    SearchResult _searchResult;

    int _enPassantSquare;

    std::vector<engine::move::Undo> _undoStack;

    uint16_t _killerMoves[engine::move::MAX_KILLER_MOVES][engine::move::MAX_PLY];
    uint16_t _historyMoves[2][6][64];

    int _pvLength[64];
    uint16_t _pvTable[engine::move::MAX_PLY][engine::move::MAX_PLY];

    int _repetitionIndex;
    uint64_t _repetitionTable[_REPETITION_TABLE_SIZE];

    std::vector<uint16_t> _moveHistory;

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

    void generatePawnMoves(engine::move::Move::MoveList &moves, engine::board::ColourType side);

    void generateKnightMoves(engine::move::Move::MoveList &moves, engine::board::ColourType side);

    void generateBishopMoves(engine::move::Move::MoveList &moves, engine::board::ColourType side);

    void generateRookMoves(engine::move::Move::MoveList &moves, engine::board::ColourType side);

    void generateQueenMoves(engine::move::Move::MoveList &moves, engine::board::ColourType side);

    engine::move::Move::MoveList generateKingMoves(engine::board::ColourType side);

    void generateKingMoves(engine::move::Move::MoveList &moves, engine::board::ColourType side);

    void generateCastleMoves(engine::move::Move::MoveList &moves, engine::board::ColourType side);

    engine::move::Move::MoveList generateCaptures(engine::board::ColourType side);

    void generatePawnCaptures(engine::move::Move::MoveList &captures, engine::board::ColourType side);

    void generateKnightCaptures(engine::move::Move::MoveList &captures, engine::board::ColourType side);

    void generateBishopCaptures(engine::move::Move::MoveList &captures, engine::board::ColourType side);

    void generateRookCaptures(engine::move::Move::MoveList &captures, engine::board::ColourType side);

    void generateQueenCaptures(engine::move::Move::MoveList &captures, engine::board::ColourType side);

    void generateKingCaptures(engine::move::Move::MoveList &captures, engine::board::ColourType side);

    bool isInCheck(engine::board::ColourType side);

    bool isSquareAttacked(int square, engine::board::ColourType side);

    bool areSquaresAttacked(uint64_t squares, engine::board::ColourType side);

    void updateCastleRights();

    void updateCastleRights(engine::board::ColourType side);

    void unmakeMove(uint16_t &move);

    void makeNullMove();

    void unmakeNullMove();

    FORCE_INLINE void makeQuietMove(int from, int to, engine::board::PieceType fromPiece);

    FORCE_INLINE void makeCaptureMove(int from, int to, engine::move::Undo &undo, engine::board::PieceType fromPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void makeDoublePawnMove(int from, int to, engine::board::PieceType fromPiece);

    FORCE_INLINE void makeEnPassantMove(int from, int to, engine::board::PieceType fromPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void makeQueenCastleMove(int from, int to, engine::board::PieceType fromPiece);

    FORCE_INLINE void makeKingCastleMove(int from, int to, engine::board::PieceType fromPiece);

    FORCE_INLINE void makePromotionQuietMove(int from, int to, engine::board::PieceType promotionPiece);

    FORCE_INLINE void makePromotionCaptureMove(int from, int to, engine::board::PieceType promotionPiece, engine::move::Undo &undo, engine::board::ColourType otherSide);

    FORCE_INLINE void unmakeQuietMove(int from, int to, engine::board::PieceType toPiece);

    FORCE_INLINE void unmakeCaptureMove(int from, int to, const engine::move::Undo &undo, engine::board::PieceType toPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void unmakeEnPassantMove(int from, int to, engine::board::PieceType toPiece, engine::board::ColourType otherSide);

    FORCE_INLINE void unmakeQueenCastleMove(int from, int to, engine::board::PieceType toPiece);

    FORCE_INLINE void unmakeKingCastleMove(int from, int to, engine::board::PieceType toPiece);

    FORCE_INLINE void unmakePromotionQuietMove(int from, int to, engine::board::PieceType promotionPiece);

    FORCE_INLINE void unmakePromotionCaptureMove(int from, int to, engine::board::PieceType promotionPiece, const engine::move::Undo &undo, engine::board::ColourType otherSide);

    FORCE_INLINE void storeKillerMove(const uint16_t move, int ply);

    FORCE_INLINE void storeHistoryMove(const uint16_t move, engine::board::ColourType side, int depth);

    FORCE_INLINE void storePVMove(const uint16_t move, int ply);

    void orderMoves(engine::move::Move::MoveList &moves, uint16_t ttMove, engine::board::ColourType side, int ply);

    int seeMove(int from, int to, engine::board::PieceType toPiece, engine::board::ColourType side);

    int see(int to, engine::board::PieceType toPiece, engine::board::ColourType side);

    FORCE_INLINE bool isNMP(bool isPVNode, bool isParentInCheck, int depth, int ply);

    FORCE_INLINE bool isRazoring(bool isPVNode, bool isParentInCheck, int depth);

    FORCE_INLINE bool isLMR(const uint16_t move, bool isPVNode, bool isParentInCheck);

    FORCE_INLINE int probeTranspositionTable(int alpha, int beta, int depth, int ply, uint16_t &bestMove);

    FORCE_INLINE void recordTranspositionTableEntry(int score, int depth, engine::hash::Transposition::NodeType nodeType, int ply, uint16_t bestMove);

    FORCE_INLINE bool isRepetition(int ply);

    uint16_t getNotatedMove(std::string &moveString);

    void searchIterative(int depth);

    void searchRoot(int depth);

    int search(int alpha, int beta, int depth, int ply, bool canNMP);

    int quiescence(int alpha, int beta, int ply);

    int evaluate(engine::board::ColourType side);

    int evaluatePesto(engine::board::ColourType side);

    FORCE_INLINE int getGamePhaseScore();

    uint64_t perft(int depth);

    void reset();
};

} // namespace engine
