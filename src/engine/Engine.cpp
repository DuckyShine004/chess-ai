#include <chrono>
#include <cstring>

#include "engine/Engine.hpp"

#include "engine/board/Fen.hpp"
#include "engine/board/Castle.hpp"
#include "engine/board/Square.hpp"

#include "engine/hash/Zobrist.hpp"

#include "engine/move/Order.hpp"

#include "engine/evaluation/Material.hpp"
#include "engine/evaluation/Position.hpp"

#include "engine/piece/Pawn.hpp"
#include "engine/piece/Knight.hpp"
#include "engine/piece/Bishop.hpp"
#include "engine/piece/Rook.hpp"
#include "engine/piece/Queen.hpp"
#include "engine/piece/King.hpp"

#include "utility/BitUtility.hpp"
#include "utility/BoardUtility.hpp"
#include "utility/StringUtility.hpp"

using namespace engine::board;

using namespace engine::piece;

using namespace engine::hash;

using namespace engine::move;

using namespace engine::move::Move;

using namespace engine::evaluation;

using namespace utility;

namespace engine {

Engine::Engine() : _ply(0) {
    this->initialise();

    this->parse(INITIAL_POSITION);
}

void Engine::parse(const char *fen) {
    this->reset();

    std::vector<std::string> states = StringUtility::splitStringByWhiteSpace(fen);

    this->parseFenPosition(states[0]);
    this->parseFenSide(states[1]);
    this->parseFenCastleRights(states[2]);
    this->parseFenEnPassantSquare(states[3]);
    this->parseFenHalfMove(states[4]);
    this->parseFenFullMove(states[5]);
}

void Engine::run() {
    uint16_t &move = this->getMove();

    this->makeMove(move);
}

uint16_t &Engine::getMove() {
    this->searchRoot(this->_SEARCH_DEPTH);

    // if (!this->_searchResult.isMoveFound) {
    //     throw std::runtime_error("Engine could not find move...");
    // }

    return this->_searchResult.bestMove;
}

void Engine::switchSide() {
    this->_side = BoardUtility::getOtherSide(this->_side);
}

int Engine::getPly() {
    return this->_ply;
}

PieceType Engine::getPiece(int square, ColourType side) {
    return BoardUtility::getPiece(this->_bitboards, square, side);
}

void Engine::runPerft(int depth) {
    auto start = std::chrono::high_resolution_clock::now();

    int nodes = this->perft(depth);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Time: " << elapsed.count() << " ms | Nodes: " << nodes << '\n';
}

ColourType Engine::getSide() {
    return this->_side;
}

void Engine::printBoard() {
    BoardUtility::printBoard(this->_bitboards);
}

void Engine::initialise() {
    Bishop::initialise();

    Rook::initialise();

    Zobrist::initialise();

    for (int square = 0; square < 64; ++square) {
        this->_PAWN_ATTACKS[0][square] = Pawn::getAttacks(square, ColourType::WHITE);
        this->_PAWN_ATTACKS[1][square] = Pawn::getAttacks(square, ColourType::BLACK);

        this->_KNIGHT_ATTACKS[square] = Knight::getAttacks(square);

        this->_KING_ATTACKS[square] = King::getAttacks(square);
    }
}

void Engine::parseFenPosition(std::string &position) {
    int rank = 7;
    int file = 0;

    for (const char letter : position) {
        if (letter == '/') {
            file = 0;
            --rank;
        } else {
            if (StringUtility::isLetter(letter)) {
                this->createFenPiece(rank, file, letter);
                ++file;
            } else {
                file += letter - '0';
            }
        }
    }
}

void Engine::parseFenSide(std::string &side) {
    this->_side = (side == "w") ? ColourType::WHITE : ColourType::BLACK;
}

void Engine::parseFenCastleRights(std::string &castleRights) {
    this->_castleRights = 0;

    if (castleRights == "-") {
        return;
    }

    for (char c : castleRights) {
        switch (c) {
        case 'K':
            this->_castleRights |= CASTLE_MASK[Castle::WHITE_KING];
            break;
        case 'Q':
            this->_castleRights |= CASTLE_MASK[Castle::WHITE_QUEEN];
            break;
        case 'k':
            this->_castleRights |= CASTLE_MASK[Castle::BLACK_KING];
            break;
        case 'q':
            this->_castleRights |= CASTLE_MASK[Castle::BLACK_QUEEN];
            break;
        default:
            break;
        }
    }
}

void Engine::parseFenEnPassantSquare(std::string &enPassantSquare) {
    this->_enPassantSquare = (enPassantSquare == "-") ? -1 : BoardUtility::getSquareFromPosition(enPassantSquare);
}

void Engine::parseFenHalfMove(std::string &halfMove) {
    this->_halfMove = (halfMove == "-") ? 1 : std::stoi(halfMove);
}

void Engine::parseFenFullMove(std::string &fullMove) {
    this->_fullMove = (fullMove == "-") ? 1 : std::stoi(fullMove);
}

void Engine::createFenPiece(int rank, int file, char letter) {
    ColourType side = isupper(letter) ? ColourType::WHITE : ColourType::BLACK;
    PieceType piece = PieceType::EMPTY;

    letter = tolower(letter);

    switch (letter) {
    case 'p':
        piece = PieceType::PAWN;
        break;
    case 'n':
        piece = PieceType::KNIGHT;
        break;
    case 'b':
        piece = PieceType::BISHOP;
        break;
    case 'r':
        piece = PieceType::ROOK;
        break;
    case 'q':
        piece = PieceType::QUEEN;
        break;
    case 'k':
        piece = PieceType::KING;
        break;
    default:
        piece = PieceType::EMPTY;
        break;
    }

    if (piece == PieceType::EMPTY) {
        return;
    }

    this->createPiece(rank, file, piece, side);
}

void Engine::createPiece(int rank, int file, ColourType side) {
    int square = BoardUtility::getSquare(rank, file);

    PieceType piece = BoardUtility::getPiece(this->_bitboards, square, side);

    this->createPiece(square, piece, side);
}

void Engine::createPiece(int square, ColourType side) {
    PieceType piece = BoardUtility::getPiece(this->_bitboards, square, side);

    this->createPiece(square, piece, side);
}

void Engine::createPiece(int rank, int file, PieceType piece, ColourType side) {
    this->createPiece(BoardUtility::getSquare(rank, file), piece, side);
}

void Engine::createPiece(int square, PieceType piece, ColourType side) {
    const uint64_t bitboard_square = BITBOARD_SQUARES[square];

    this->_bitboards[side][piece] |= bitboard_square;

    this->_occupancies[side] |= bitboard_square;

    this->_occupancyBoth |= bitboard_square;
}

void Engine::removePiece(int rank, int file, ColourType side) {
    int square = BoardUtility::getSquare(rank, file);

    PieceType piece = BoardUtility::getPiece(this->_bitboards, square, side);

    this->removePiece(square, piece, side);
}

void Engine::removePiece(int square, ColourType side) {
    PieceType piece = BoardUtility::getPiece(this->_bitboards, square, side);

    this->removePiece(square, piece, side);
}

void Engine::removePiece(int rank, int file, PieceType piece, ColourType side) {
    this->removePiece(BoardUtility::getSquare(rank, file), piece, side);
}

void Engine::removePiece(int square, PieceType piece, ColourType side) {
    const uint64_t inverted_bitboard_square = INVERTED_BITBOARD_SQUARES[square];

    this->_bitboards[side][piece] &= inverted_bitboard_square;

    this->_occupancies[side] &= inverted_bitboard_square;

    this->_occupancyBoth &= inverted_bitboard_square;
}

MoveList Engine::generateMoves(ColourType side) {
    MoveList moves;

    this->generatePawnMoves(moves, side);
    this->generateKnightMoves(moves, side);
    this->generateBishopMoves(moves, side);
    this->generateRookMoves(moves, side);
    this->generateQueenMoves(moves, side);
    this->generateKingMoves(moves, side);

    return moves;
}

void Engine::generatePawnMoves(MoveList &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t pawns = this->_bitboards[side][PieceType::PAWN];

    uint64_t empty = ~this->_occupancyBoth;

    while (pawns) {
        int from = BitUtility::popLSB(pawns);

        if (Pawn::canSinglePush(from, side, empty)) {
            int to = Pawn::singlePush(from, side);

            if (Pawn::ENEMY_BACK_RANK[side] & (BITBOARD_SQUARES[to])) {
                moves.add(from, to, MoveType::KNIGHT_PROMOTION);
                moves.add(from, to, MoveType::BISHOP_PROMOTION);
                moves.add(from, to, MoveType::ROOK_PROMOTION);
                moves.add(from, to, MoveType::QUEEN_PROMOTION);
            } else {
                moves.add(from, to, MoveType::QUIET);
            }
        }

        if (Pawn::canDoublePush(from, side, empty)) {
            int to = Pawn::doublePush(from, side);

            moves.add(from, to, MoveType::DOUBLE_PAWN);
        }

        uint64_t captureMoves = this->_PAWN_ATTACKS[side][from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            if (Pawn::ENEMY_BACK_RANK[side] & BITBOARD_SQUARES[to]) {
                moves.add(from, to, MoveType::KNIGHT_PROMOTION_CAPTURE);
                moves.add(from, to, MoveType::BISHOP_PROMOTION_CAPTURE);
                moves.add(from, to, MoveType::ROOK_PROMOTION_CAPTURE);
                moves.add(from, to, MoveType::QUEEN_PROMOTION_CAPTURE);
            } else {
                moves.add(from, to, MoveType::CAPTURE);
            }
        }

        if (this->_enPassantSquare != -1) {
            uint64_t enPassantMoves = this->_PAWN_ATTACKS[side][from] & BITBOARD_SQUARES[this->_enPassantSquare];

            while (enPassantMoves) {
                int to = BitUtility::popLSB(enPassantMoves);

                moves.add(from, to, MoveType::EN_PASSANT);
            }
        }
    }
}

void Engine::generateKnightMoves(MoveList &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t knights = this->_bitboards[side][PieceType::KNIGHT];

    uint64_t empty = ~this->_occupancyBoth;

    while (knights) {
        int from = BitUtility::popLSB(knights);

        uint64_t quietMoves = this->_KNIGHT_ATTACKS[from] & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            moves.add(from, to, MoveType::QUIET);
        }

        uint64_t captureMoves = this->_KNIGHT_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            moves.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateBishopMoves(MoveList &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t bishops = this->_bitboards[side][PieceType::BISHOP];

    uint64_t empty = ~this->_occupancyBoth;

    while (bishops) {
        int from = BitUtility::popLSB(bishops);

        uint64_t attacks = Bishop::getAttacks(from, this->_occupancyBoth);

        uint64_t quietMoves = attacks & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            moves.add(from, to, MoveType::QUIET);
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            moves.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateRookMoves(MoveList &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t rooks = this->_bitboards[side][PieceType::ROOK];

    uint64_t empty = ~this->_occupancyBoth;

    while (rooks) {
        int from = BitUtility::popLSB(rooks);

        uint64_t attacks = Rook::getAttacks(from, this->_occupancyBoth);

        uint64_t quietMoves = attacks & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            moves.add(from, to, MoveType::QUIET);
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            moves.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateQueenMoves(MoveList &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t queens = this->_bitboards[side][PieceType::QUEEN];

    uint64_t empty = ~this->_occupancyBoth;

    while (queens) {
        int from = BitUtility::popLSB(queens);

        uint64_t attacks = Queen::getAttacks(from, this->_occupancyBoth);

        uint64_t quietMoves = attacks & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            moves.add(from, to, MoveType::QUIET);
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            moves.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateKingMoves(MoveList &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t kings = this->_bitboards[side][PieceType::KING];

    uint64_t empty = ~this->_occupancyBoth;

    while (kings) {
        int from = BitUtility::popLSB(kings);

        uint64_t quietMoves = this->_KING_ATTACKS[from] & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            moves.add(from, to, MoveType::QUIET);
        }

        uint64_t captureMoves = this->_KING_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            moves.add(from, to, MoveType::CAPTURE);
        }
    }

    this->generateCastleMoves(moves, side);
}

void Engine::generateCastleMoves(MoveList &moves, ColourType side) {
    const Castle kingSide = (side == ColourType::WHITE) ? Castle::WHITE_KING : Castle::BLACK_KING;
    const Castle queenSide = (side == ColourType::WHITE) ? Castle::WHITE_QUEEN : Castle::BLACK_QUEEN;

    int kingOriginSquare = KING_ORIGIN_SQUARES[side];

    if (!BitUtility::isBitSet(this->_bitboards[side][PieceType::KING], kingOriginSquare)) {
        return;
    }

    if (this->_castleRights & CASTLE_MASK[kingSide]) {
        bool isRookAtOrigin = BitUtility::isBitSet(this->_bitboards[side][PieceType::ROOK], ROOK_ORIGIN_SQUARES[kingSide]);

        bool isEmpty = (this->_occupancyBoth & CASTLE_EMPTY_MASK[kingSide]) == 0ULL;

        bool isSafe = !this->areSquaresAttacked(CASTLE_SAFE_MASK[kingSide], side);

        if (isRookAtOrigin && isEmpty && isSafe) {
            moves.add(kingOriginSquare, KING_TO_SQUARES[kingSide], MoveType::KING_CASTLE);
        }
    }

    if (this->_castleRights & CASTLE_MASK[queenSide]) {
        bool isRookAtOrigin = BitUtility::isBitSet(this->_bitboards[side][PieceType::ROOK], ROOK_ORIGIN_SQUARES[queenSide]);

        bool isEmpty = (this->_occupancyBoth & CASTLE_EMPTY_MASK[queenSide]) == 0ULL;

        bool isSafe = !this->areSquaresAttacked(CASTLE_SAFE_MASK[queenSide], side);

        if (isRookAtOrigin && isEmpty && isSafe) {
            moves.add(kingOriginSquare, KING_TO_SQUARES[queenSide], MoveType::QUEEN_CASTLE);
        }
    }
}

MoveList Engine::generateCaptures(ColourType side) {
    MoveList captures;

    this->generatePawnCaptures(captures, side);
    this->generateKnightCaptures(captures, side);
    this->generateBishopCaptures(captures, side);
    this->generateRookCaptures(captures, side);
    this->generateQueenCaptures(captures, side);
    this->generateKingCaptures(captures, side);

    return captures;
}

void Engine::generatePawnCaptures(MoveList &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t pawns = this->_bitboards[side][PieceType::PAWN];

    uint64_t empty = ~this->_occupancyBoth;

    while (pawns) {
        int from = BitUtility::popLSB(pawns);

        uint64_t captureMoves = this->_PAWN_ATTACKS[side][from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            if (Pawn::ENEMY_BACK_RANK[side] & BITBOARD_SQUARES[to]) {
                captures.add(from, to, MoveType::KNIGHT_PROMOTION_CAPTURE);
                captures.add(from, to, MoveType::BISHOP_PROMOTION_CAPTURE);
                captures.add(from, to, MoveType::ROOK_PROMOTION_CAPTURE);
                captures.add(from, to, MoveType::QUEEN_PROMOTION_CAPTURE);
            } else {
                captures.add(from, to, MoveType::CAPTURE);
            }
        }

        if (this->_enPassantSquare != -1) {
            uint64_t enPassantMoves = this->_PAWN_ATTACKS[side][from] & BITBOARD_SQUARES[this->_enPassantSquare];

            while (enPassantMoves) {
                int to = BitUtility::popLSB(enPassantMoves);

                captures.add(from, to, MoveType::EN_PASSANT);
            }
        }
    }
}

void Engine::generateKnightCaptures(MoveList &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t knights = this->_bitboards[side][PieceType::KNIGHT];

    uint64_t empty = ~this->_occupancyBoth;

    while (knights) {
        int from = BitUtility::popLSB(knights);

        uint64_t captureMoves = this->_KNIGHT_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            captures.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateBishopCaptures(MoveList &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t bishops = this->_bitboards[side][PieceType::BISHOP];

    uint64_t empty = ~this->_occupancyBoth;

    while (bishops) {
        int from = BitUtility::popLSB(bishops);

        uint64_t attacks = Bishop::getAttacks(from, this->_occupancyBoth);

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            captures.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateRookCaptures(MoveList &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t rooks = this->_bitboards[side][PieceType::ROOK];

    uint64_t empty = ~this->_occupancyBoth;

    while (rooks) {
        int from = BitUtility::popLSB(rooks);

        uint64_t attacks = Rook::getAttacks(from, this->_occupancyBoth);

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            captures.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateQueenCaptures(MoveList &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t queens = this->_bitboards[side][PieceType::QUEEN];

    uint64_t empty = ~this->_occupancyBoth;

    while (queens) {
        int from = BitUtility::popLSB(queens);

        uint64_t attacks = Queen::getAttacks(from, this->_occupancyBoth);

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            captures.add(from, to, MoveType::CAPTURE);
        }
    }
}

void Engine::generateKingCaptures(MoveList &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t kings = this->_bitboards[side][PieceType::KING];

    uint64_t empty = ~this->_occupancyBoth;

    while (kings) {
        int from = BitUtility::popLSB(kings);

        uint64_t captureMoves = this->_KING_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            captures.add(from, to, MoveType::CAPTURE);
        }
    }
}

bool Engine::isMoveLegal(uint16_t &move, ColourType side) {
    this->makeMove(move);

    bool isInCheck = this->isInCheck(side);

    this->unmakeMove(move);

    return isInCheck;
}

bool Engine::isInCheck(ColourType side) {
    int kingSquare = this->getKingSquare(side);

    return !this->isSquareAttacked(kingSquare, side);
}

bool Engine::isSquareAttacked(int square, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    if (this->_PAWN_ATTACKS[side][square] & this->_bitboards[otherSide][PieceType::PAWN]) {
        return true;
    }

    if (this->_KNIGHT_ATTACKS[square] & this->_bitboards[otherSide][PieceType::KNIGHT]) {
        return true;
    }

    if (this->_KING_ATTACKS[square] & this->_bitboards[otherSide][PieceType::KING]) {
        return true;
    }

    uint64_t bishopAttacks = Bishop::getAttacks(square, this->_occupancyBoth);

    if (bishopAttacks & (this->_bitboards[otherSide][PieceType::BISHOP] | this->_bitboards[otherSide][PieceType::QUEEN])) {
        return true;
    }

    uint64_t rookAttacks = Rook::getAttacks(square, this->_occupancyBoth);

    if (rookAttacks & (this->_bitboards[otherSide][PieceType::ROOK] | this->_bitboards[otherSide][PieceType::QUEEN])) {
        return true;
    }

    return false;
}

bool Engine::areSquaresAttacked(uint64_t squares, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    // PERF: Optimise later if it becomes an issue
    while (squares) {
        int square = BitUtility::popLSB(squares);

        if (this->isSquareAttacked(square, side)) {
            return true;
        }
    }

    return false;
}

int Engine::getKingSquare(ColourType side) {
    return BitUtility::getLSBIndex(this->_bitboards[side][PieceType::KING]);
}

void Engine::updateCastleRights() {
    this->updateCastleRights(this->_side);
    this->updateCastleRights(BoardUtility::getOtherSide(this->_side));
}

void Engine::updateCastleRights(ColourType side) {
    const Castle kingSide = (side == ColourType::WHITE) ? Castle::WHITE_KING : Castle::BLACK_KING;
    const Castle queenSide = (side == ColourType::WHITE) ? Castle::WHITE_QUEEN : Castle::BLACK_QUEEN;

    int kingOriginSquare = KING_ORIGIN_SQUARES[side];

    if (!BitUtility::isBitSet(this->_bitboards[side][PieceType::KING], KING_ORIGIN_SQUARES[side])) {
        BitUtility::clearBit(this->_castleRights, kingSide);
        BitUtility::clearBit(this->_castleRights, queenSide);

        return;
    }

    if (!BitUtility::isBitSet(this->_bitboards[side][PieceType::ROOK], ROOK_ORIGIN_SQUARES[kingSide])) {
        BitUtility::clearBit(this->_castleRights, kingSide);
    }

    if (!BitUtility::isBitSet(this->_bitboards[side][PieceType::ROOK], ROOK_ORIGIN_SQUARES[queenSide])) {
        BitUtility::clearBit(this->_castleRights, queenSide);
    }
}

void Engine::makeMove(uint16_t &move) {
    Undo undo;

    undo.castleRights = this->_castleRights;
    undo.enPassantSquare = this->_enPassantSquare;
    undo.halfMove = this->_halfMove;

    this->_enPassantSquare = -1;

    int from = Move::getFrom(move);
    int to = Move::getTo(move);
    PieceType promotionPiece = Move::getPromotionPiece(move);

    ColourType otherSide = BoardUtility::getOtherSide(this->_side);

    PieceType fromPiece = BoardUtility::getPiece(this->_bitboards, from, this->_side);

    bool isCapture = false;

    if (Move::isQuiet(move)) {
        this->makeQuietMove(from, to, fromPiece);
    } else if (Move::isCapture(move)) {
        this->makeCaptureMove(from, to, undo, fromPiece, otherSide);

        isCapture = true;
    } else if (Move::isDoublePawn(move)) {
        this->makeDoublePawnMove(from, to, fromPiece);
    } else if (Move::isEnPassant(move)) {
        this->makeEnPassantMove(from, to, fromPiece, otherSide);
    } else if (Move::isQueenCastle(move)) {
        this->makeQueenCastleMove(from, to, fromPiece);
    } else if (Move::isKingCastle(move)) {
        this->makeKingCastleMove(from, to, fromPiece);
    } else if (Move::isPromotionQuiet(move)) {
        this->makePromotionQuietMove(from, to, promotionPiece);
    } else if (Move::isPromotionCapture(move)) {
        this->makePromotionCaptureMove(from, to, promotionPiece, undo, otherSide);
    }

    this->updateCastleRights();

    if (fromPiece == PieceType::PAWN || isCapture) {
        this->_halfMove = 0;
    } else {
        ++this->_halfMove;
    }

    this->_undoStack.push_back(std::move(undo));

    this->switchSide();
}

// Switch sides immediately since we last switched at leaf node
void Engine::unmakeMove(uint16_t &move) {
    this->switchSide();

    const Undo &undo = this->_undoStack.back();

    int from = Move::getFrom(move);
    int to = Move::getTo(move);
    PieceType promotionPiece = Move::getPromotionPiece(move);

    ColourType otherSide = BoardUtility::getOtherSide(this->_side);

    PieceType toPiece = BoardUtility::getPiece(this->_bitboards, to, this->_side);

    if (Move::isQuiet(move) || Move::isDoublePawn(move)) {
        this->unmakeQuietMove(from, to, toPiece);
    } else if (Move::isCapture(move)) {
        this->unmakeCaptureMove(from, to, undo, toPiece, otherSide);
    } else if (Move::isEnPassant(move)) {
        this->unmakeEnPassantMove(from, to, toPiece, otherSide);
    } else if (Move::isQueenCastle(move)) {
        this->unmakeQueenCastleMove(from, to, toPiece);
    } else if (Move::isKingCastle(move)) {
        this->unmakeKingCastleMove(from, to, toPiece);
    } else if (Move::isPromotionQuiet(move)) {
        this->unmakePromotionQuietMove(from, to, promotionPiece);
    } else if (Move::isPromotionCapture(move)) {
        this->unmakePromotionCaptureMove(from, to, promotionPiece, undo, otherSide);
    }

    this->_castleRights = undo.castleRights;
    this->_enPassantSquare = undo.enPassantSquare;
    this->_halfMove = undo.halfMove;

    this->_undoStack.pop_back();
}

void Engine::makeQuietMove(int from, int to, PieceType fromPiece) {
    this->removePiece(from, fromPiece, this->_side);

    this->createPiece(to, fromPiece, this->_side);
}

void Engine::makeCaptureMove(int from, int to, Undo &undo, PieceType fromPiece, ColourType otherSide) {
    PieceType capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

    this->removePiece(from, fromPiece, this->_side);

    this->removePiece(to, capturedPiece, otherSide);

    this->createPiece(to, fromPiece, this->_side);

    undo.capturedPiece = capturedPiece;
}

void Engine::makeDoublePawnMove(int from, int to, PieceType fromPiece) {
    int fromFile = BoardUtility::getFile(from);

    this->_enPassantSquare = EN_PASSANT_SQUARES[this->_side][fromFile];

    this->removePiece(from, fromPiece, this->_side);

    this->createPiece(to, fromPiece, this->_side);
}

void Engine::makeEnPassantMove(int from, int to, PieceType fromPiece, ColourType otherSide) {
    int toFile = BoardUtility::getFile(to);

    this->removePiece(EN_PASSANT_CAPTURE_SQUARES[this->_side][toFile], PieceType::PAWN, otherSide);

    this->removePiece(from, fromPiece, this->_side);

    this->createPiece(to, fromPiece, this->_side);
}

void Engine::makeQueenCastleMove(int from, int to, PieceType fromPiece) {
    const Castle queenSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_QUEEN : Castle::BLACK_QUEEN;

    this->removePiece(ROOK_ORIGIN_SQUARES[queenSide], PieceType::ROOK, this->_side);

    this->removePiece(from, fromPiece, this->_side);

    this->createPiece(ROOK_TO_SQUARES[queenSide], PieceType::ROOK, this->_side);

    this->createPiece(to, fromPiece, this->_side);
}

void Engine::makeKingCastleMove(int from, int to, PieceType fromPiece) {
    const Castle kingSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_KING : Castle::BLACK_KING;

    this->removePiece(ROOK_ORIGIN_SQUARES[kingSide], PieceType::ROOK, this->_side);

    this->removePiece(from, fromPiece, this->_side);

    this->createPiece(ROOK_TO_SQUARES[kingSide], PieceType::ROOK, this->_side);

    this->createPiece(to, fromPiece, this->_side);
}

void Engine::makePromotionQuietMove(int from, int to, PieceType promotionPiece) {
    this->removePiece(from, PieceType::PAWN, this->_side);

    this->createPiece(to, promotionPiece, this->_side);
}

void Engine::makePromotionCaptureMove(int from, int to, PieceType promotionPiece, Undo &undo, ColourType otherSide) {
    PieceType capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

    this->removePiece(from, PieceType::PAWN, this->_side);

    this->removePiece(to, capturedPiece, otherSide);

    this->createPiece(to, promotionPiece, this->_side);

    undo.capturedPiece = capturedPiece;
}

void Engine::unmakeQuietMove(int from, int to, PieceType toPiece) {
    this->removePiece(to, toPiece, this->_side);

    this->createPiece(from, toPiece, this->_side);
}

void Engine::unmakeCaptureMove(int from, int to, const Undo &undo, PieceType toPiece, ColourType otherSide) {
    this->removePiece(to, toPiece, this->_side);

    this->createPiece(from, toPiece, this->_side);

    this->createPiece(to, undo.capturedPiece, otherSide);
}

void Engine::unmakeEnPassantMove(int from, int to, PieceType toPiece, ColourType otherSide) {
    int toFile = BoardUtility::getFile(to);

    this->removePiece(to, toPiece, this->_side);

    this->createPiece(from, toPiece, this->_side);

    this->createPiece(EN_PASSANT_CAPTURE_SQUARES[this->_side][toFile], PieceType::PAWN, otherSide);
}

void Engine::unmakeQueenCastleMove(int from, int to, PieceType toPiece) {
    const Castle queenSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_QUEEN : Castle::BLACK_QUEEN;

    this->removePiece(ROOK_TO_SQUARES[queenSide], PieceType::ROOK, this->_side);

    this->removePiece(to, toPiece, this->_side);

    this->createPiece(ROOK_ORIGIN_SQUARES[queenSide], PieceType::ROOK, this->_side);

    this->createPiece(from, toPiece, this->_side);
}

void Engine::unmakeKingCastleMove(int from, int to, PieceType toPiece) {
    const Castle kingSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_KING : Castle::BLACK_KING;

    this->removePiece(ROOK_TO_SQUARES[kingSide], PieceType::ROOK, this->_side);

    this->removePiece(to, toPiece, this->_side);

    this->createPiece(ROOK_ORIGIN_SQUARES[kingSide], PieceType::ROOK, this->_side);

    this->createPiece(from, toPiece, this->_side);
}

void Engine::unmakePromotionQuietMove(int from, int to, PieceType promotionPiece) {
    this->removePiece(to, promotionPiece, this->_side);

    this->createPiece(from, PieceType::PAWN, this->_side);
}

void Engine::unmakePromotionCaptureMove(int from, int to, PieceType promotionPiece, const Undo &undo, ColourType otherSide) {
    this->removePiece(to, promotionPiece, this->_side);

    this->createPiece(from, PieceType::PAWN, this->_side);

    this->createPiece(to, undo.capturedPiece, otherSide);
}

// TODO: Sort moves
// MVV-LVA [*]
// Killer Moves []
void Engine::orderMoves(Move::MoveList &moves, ColourType side) {
    int scores[256];

    ColourType otherSide = BoardUtility::getOtherSide(side);

    for (int i = 0; i < moves.size; ++i) {
        const uint16_t move = moves.moves[i];

        int from = Move::getFrom(move);
        int to = Move::getTo(move);

        PieceType fromPiece = BoardUtility::getPiece(this->_bitboards, from, side);

        scores[i] = 0;

        if (Move::isGeneralCapture(move)) {
            PieceType toPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            scores[i] += MVV_LVA[fromPiece][toPiece];
        }
    }

    for (int i = 0; i < moves.size; ++i) {
        int bestIndex = i;

        for (int j = i + 1; j < moves.size; ++j) {
            if (scores[j] > scores[bestIndex]) {
                bestIndex = j;
            }
        }

        std::swap(scores[i], scores[bestIndex]);

        std::swap(moves.moves[i], moves.moves[bestIndex]);
    }
}

void Engine::searchRoot(int depth) {
    this->_searchResult.clear();

    int alpha = -INT_MAX;
    int beta = INT_MAX;

    MoveList moves = this->generateMoves(this->_side);

    this->orderMoves(moves, this->_side);

    for (int i = 0; i < moves.size; ++i) {
        uint16_t &move = moves.moves[i];

        if (!this->isMoveLegal(move, this->_side)) {
            continue;
        }

        if (!this->_searchResult.isMoveFound) {
            this->_searchResult.bestMove = move;

            this->_searchResult.isMoveFound = true;
        }

        this->makeMove(move);

        int score = -this->search(-beta, -alpha, depth - 1);

        this->unmakeMove(move);

        if (score > this->_searchResult.bestScore) {
            this->_searchResult.bestScore = score;

            this->_searchResult.bestMove = move;

            this->_searchResult.isMoveFound = true;

            if (score > alpha) {
                alpha = score;
            }
        }

        if (score >= beta) {
            break;
        }
    }
}

int Engine::search(int alpha, int beta, int depth) {
    if (depth == 0) {
        return this->quiescence(alpha, beta);
    }

    // TODO: Implement null move pruning- condition
    // if (1) {
    //     int reduction = 2; // NMP reduction

    //     // Make null move
    //     int nullScore = -this->search(-beta, -beta + 1, depth - reduction);
    //     // Undo null move

    //     if (nullScore >= beta) {
    //         return nullScore;
    //     }
    // }
    //
    // the side to move is in check
    // the side to move has only its king and pawns remaining
    // the side to move has a small number of pieces remaining
    // the previous move in the search was also a null move.

    int bestScore = -INT_MAX;

    MoveList moves = this->generateMoves(this->_side);

    this->orderMoves(moves, this->_side);

    for (int i = 0; i < moves.size; ++i) {
        uint16_t &move = moves.moves[i];

        if (!this->isMoveLegal(move, this->_side)) {
            continue;
        }

        this->makeMove(move);

        int score = -this->search(-beta, -alpha, depth - 1);

        this->unmakeMove(move);

        if (score > bestScore) {
            bestScore = score;

            if (score > alpha) {
                alpha = score;
            }
        }

        if (score >= beta) {
            break;
        }
    }

    return bestScore;
}

int Engine::quiescence(int alpha, int beta) {
    int bestScore = this->evaluate(this->_side);

    if (bestScore >= beta) {
        return bestScore;
    }

    if (bestScore > alpha) {
        alpha = bestScore;
    }

    MoveList captures = this->generateCaptures(this->_side);

    this->orderMoves(captures, this->_side);

    for (int i = 0; i < captures.size; ++i) {
        uint16_t &capture = captures.moves[i];

        if (!this->isMoveLegal(capture, this->_side)) {
            continue;
        }

        this->makeMove(capture);

        int score = -this->quiescence(-beta, -alpha);

        this->unmakeMove(capture);

        if (score >= beta) {
            return score;
        }

        if (score > bestScore) {
            bestScore = score;
        }

        if (score > alpha) {
            alpha = score;
        }
    }

    return bestScore;
}

// TODO: implement mobility score
int Engine::evaluate(ColourType side) {
    int score = 0;

    for (uint8_t piece = PieceType::PAWN; piece <= PieceType::KING; ++piece) {
        uint64_t whitePieces = this->_bitboards[ColourType::WHITE][piece];

        while (whitePieces) {
            int square = BitUtility::popLSB(whitePieces);

            score += POSITION_TABLES[piece][square];
        }

        uint64_t blackPieces = this->_bitboards[ColourType::BLACK][piece];

        while (blackPieces) {
            int square = BitUtility::popLSB(blackPieces);

            score -= POSITION_TABLES[piece][MIRROR[square]];
        }

        int materialDifference = BitUtility::popCount(this->_bitboards[ColourType::WHITE][piece]) - BitUtility::popCount(this->_bitboards[ColourType::BLACK][piece]);

        score += MATERIAL_TABLE[piece] * materialDifference;
    }

    return (side == ColourType::WHITE) ? score : -score;
}

int Engine::perft(int depth) {
    if (depth == 0) {
        return 1;
    }

    int nodes = 0;

    MoveList moves = this->generateMoves(this->_side);

    for (int i = 0; i < moves.size; ++i) {
        uint16_t &move = moves.moves[i];

        if (!this->isMoveLegal(move, this->_side)) {
            continue;
        }

        this->makeMove(move);

        nodes += this->perft(depth - 1);

        this->unmakeMove(move);
    }

    return nodes;
}

void Engine::reset() {
    memset(this->_bitboards, 0ULL, sizeof(this->_bitboards));

    memset(this->_occupancies, 0ULL, sizeof(this->_occupancies));

    this->_occupancyBoth = 0ULL;
    this->_castleRights = this->_INITIAL_CASTLE_RIGHTS;
    this->_side = ColourType::WHITE;
    this->_enPassantSquare = -1;
}

} // namespace engine
