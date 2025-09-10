#include <chrono>
#include <cstring>

#include "engine/Engine.hpp"

#include "engine/board/Fen.hpp"
#include "engine/board/Castle.hpp"
#include "engine/board/Square.hpp"

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

using namespace engine::move;

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
    Move &move = this->getMove();

    this->makeMove(move);
}

engine::move::Move &Engine::getMove() {
    this->searchRoot(this->_SEARCH_DEPTH);

    if (!this->_searchResult.isMoveFound) {
        throw std::runtime_error("Engine could not find move...");
    }

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
    Bishop::initialiseRays();

    Rook::initialiseRays();

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
            this->_castleRights |= (1ULL << Castle::WHITE_KING);
            break;
        case 'Q':
            this->_castleRights |= (1ULL << Castle::WHITE_QUEEN);
            break;
        case 'k':
            this->_castleRights |= (1ULL << Castle::BLACK_KING);
            break;
        case 'q':
            this->_castleRights |= (1ULL << Castle::BLACK_QUEEN);
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
    uint64_t bitboard_square = BITBOARD_SQUARES[square];

    this->_bitboards[side][piece] |= bitboard_square;

    this->_occupancies[side] |= bitboard_square;

    this->_occupancyBoth |= bitboard_square;

    // this->_occupancyBoth = this->_occupancies[0] | this->_occupancies[1];
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
    uint64_t bitboard_square = BITBOARD_SQUARES[square];
    this->_bitboards[side][piece] &= ~bitboard_square;

    this->_occupancies[side] &= ~bitboard_square;

    // this->_occupancyBoth &= ~bitboard_square;
    this->_occupancyBoth = this->_occupancies[0] | this->_occupancies[1];
}

// TODO: Sort moves by move type
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

            if (Pawn::ENEMY_BACK_RANK[side] & (1ULL << to)) {
                Move move(from, to, MoveType::KNIGHT_PROMOTION);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }

                move = Move(from, to, MoveType::BISHOP_PROMOTION);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }

                move = Move(from, to, MoveType::ROOK_PROMOTION);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }

                move = Move(from, to, MoveType::QUEEN_PROMOTION);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }
            } else {
                Move move(from, to, MoveType::QUIET);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }
            }
        }

        if (Pawn::canDoublePush(from, side, empty)) {
            int to = Pawn::doublePush(from, side);

            Move move(from, to, MoveType::DOUBLE_PAWN);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
        }

        uint64_t captureMoves = this->_PAWN_ATTACKS[side][from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            if (Pawn::ENEMY_BACK_RANK[side] & (1ULL << to)) {
                Move move(from, to, MoveType::KNIGHT_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }

                move = Move(from, to, MoveType::BISHOP_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }

                move = Move(from, to, MoveType::ROOK_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }

                move = Move(from, to, MoveType::QUEEN_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }
            } else {
                Move move(from, to, MoveType::CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }
            }
        }

        if (this->_enPassantSquare != -1) {
            uint64_t enPassantMoves = this->_PAWN_ATTACKS[side][from] & (1ULL << this->_enPassantSquare);

            while (enPassantMoves) {
                int to = BitUtility::popLSB(enPassantMoves);

                Move move(from, to, MoveType::EN_PASSANT);

                if (this->isMoveLegal(move, side)) {
                    moves.add(std::move(move));
                }
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

            Move move(from, to, MoveType::QUIET);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
        }

        uint64_t captureMoves = this->_KNIGHT_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
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

            Move move(from, to, MoveType::QUIET);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
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

            Move move(from, to, MoveType::QUIET);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
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

            Move move(from, to, MoveType::QUIET);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
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

            Move move(from, to, MoveType::QUIET);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
        }

        uint64_t captureMoves = this->_KING_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                moves.add(std::move(move));
            }
        }
    }

    this->generateCastleMoves(moves, side);
}

void Engine::generateCastleMoves(MoveList &moves, ColourType side) {
    const Castle kingSide = (side == ColourType::WHITE) ? Castle::WHITE_KING : Castle::BLACK_KING;
    const Castle queenSide = (side == ColourType::WHITE) ? Castle::WHITE_QUEEN : Castle::BLACK_QUEEN;

    int kingOriginSquare = KING_ORIGIN_SQUARES[side];

    // Check if the king is actually at it's origin
    if (!BitUtility::isBitSet(this->_bitboards[side][PieceType::KING], kingOriginSquare)) {
        return;
    }

    // Check if we can king side castle
    if (this->_castleRights & (1ULL << kingSide)) {
        // Check if it's actually the rook there
        bool isRookAtOrigin = BitUtility::isBitSet(this->_bitboards[side][PieceType::ROOK], ROOK_ORIGIN_SQUARES[kingSide]);

        // Check if there are pieces in between
        bool isEmpty = (this->_occupancyBoth & CASTLE_EMPTY_MASK[kingSide]) == 0ULL;

        // Check if the squares we are moving to are safe
        bool isSafe = !this->areSquaresAttacked(CASTLE_SAFE_MASK[kingSide], side);

        if (isRookAtOrigin && isEmpty && isSafe) {
            // Since we have already checked if it's safe to move to the safe squares, we don't need to perform extra king check
            moves.add(kingOriginSquare, KING_TO_SQUARES[kingSide], MoveType::KING_CASTLE);
        }
    }

    // Check if we can castle queen side
    if (this->_castleRights & (1ULL << queenSide)) {
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

            if (Pawn::ENEMY_BACK_RANK[side] & (1ULL << to)) {
                Move move(from, to, MoveType::KNIGHT_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    captures.add(std::move(move));
                }

                move = Move(from, to, MoveType::BISHOP_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    captures.add(std::move(move));
                }

                move = Move(from, to, MoveType::ROOK_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    captures.add(std::move(move));
                }
                move = Move(from, to, MoveType::QUEEN_PROMOTION_CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    captures.add(std::move(move));
                }
            } else {
                Move move(from, to, MoveType::CAPTURE);

                if (this->isMoveLegal(move, side)) {
                    captures.add(std::move(move));
                }
            }
        }

        if (this->_enPassantSquare != -1) {
            uint64_t enPassantMoves = this->_PAWN_ATTACKS[side][from] & (1ULL << this->_enPassantSquare);

            while (enPassantMoves) {
                int to = BitUtility::popLSB(enPassantMoves);

                Move move(from, to, MoveType::EN_PASSANT);

                if (this->isMoveLegal(move, side)) {
                    captures.add(std::move(move));
                }
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

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                captures.add(std::move(move));
            }
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

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                captures.add(std::move(move));
            }
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

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                captures.add(std::move(move));
            }
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

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                captures.add(std::move(move));
            }
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

            Move move(from, to, MoveType::CAPTURE);

            if (this->isMoveLegal(move, side)) {
                captures.add(std::move(move));
            }
        }
    }
}

bool Engine::isMoveLegal(Move &move, ColourType side) {
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

void Engine::makeMove(Move &move) {
    Undo undo;

    // Keep track of previous states
    undo.castleRights = this->_castleRights;
    undo.enPassantSquare = this->_enPassantSquare;

    ColourType otherSide = BoardUtility::getOtherSide(this->_side);

    PieceType fromPiece = BoardUtility::getPiece(this->_bitboards, move.from, this->_side);

    // Remove the piece from the from square
    this->removePiece(move.from, fromPiece, this->_side);

    // Create from piece in the to square
    this->createPiece(move.to, fromPiece, this->_side);

    const Castle kingSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_KING : Castle::BLACK_KING;
    const Castle queenSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_QUEEN : Castle::BLACK_QUEEN;
    // Perform the move type
    if (move.moveType == MoveType::CAPTURE) {
        // Check what the captured piece was
        PieceType capturedPiece = BoardUtility::getPiece(this->_bitboards, move.to, otherSide);

        // Remove the captured piece
        this->removePiece(move.to, capturedPiece, otherSide);

        undo.capturedPiece = capturedPiece;
    } else if (move.moveType == MoveType::DOUBLE_PAWN) {
        this->_enPassantSquare = EN_PASSANT_SQUARES[this->_side][BoardUtility::getFile(move.from)];
    } else if (move.moveType == MoveType::EN_PASSANT) {
        // Remove piece at the en passant capture square
        this->removePiece(EN_PASSANT_CAPTURE_SQUARES[this->_side][BoardUtility::getFile(move.to)], PieceType::PAWN, otherSide);
    } else if (move.moveType == MoveType::KING_CASTLE) {
        // Move the king side rook to landing square
        this->removePiece(ROOK_ORIGIN_SQUARES[kingSide], PieceType::ROOK, this->_side);
        this->createPiece(ROOK_TO_SQUARES[kingSide], PieceType::ROOK, this->_side);
    } else if (move.moveType == MoveType::QUEEN_CASTLE) {
        // Move the queen side rook
        this->removePiece(ROOK_ORIGIN_SQUARES[queenSide], PieceType::ROOK, this->_side);
        this->createPiece(ROOK_TO_SQUARES[queenSide], PieceType::ROOK, this->_side);
    } else if (move.isPromotionQuiet()) {
        // Remove from piece in the to square
        this->removePiece(move.to, fromPiece, this->_side);

        // Replace with promotion piece
        this->createPiece(move.to, move.getPromotionPiece(), this->_side);
    } else if (move.isPromotionCapture()) {
        // Check what the captured piece was
        PieceType capturedPiece = BoardUtility::getPiece(this->_bitboards, move.to, otherSide);

        // Remove the captured piece
        this->removePiece(move.to, capturedPiece, otherSide);

        // Remove the pawn at the to square
        this->removePiece(move.to, fromPiece, this->_side);

        // Set the promoted piece
        this->createPiece(move.to, move.getPromotionPiece(), this->_side);

        undo.capturedPiece = capturedPiece;
    }

    // Update castle rights for both sides (more of checking if king or rook has moved)
    this->updateCastleRights();

    // If we didn't do a double pawn push, then en passant square should reset
    if (move.moveType != MoveType::DOUBLE_PAWN) {
        this->_enPassantSquare = -1;
    }

    this->_undoStack.push_back(std::move(undo));

    this->switchSide();
}

void Engine::unmakeMove(Move &move) {
    // Switch immediately, e.g., if white was leaf,
    // then we want to switch back to get back to white
    // Because at leaf, we switched sides to black
    this->switchSide();

    Undo &undo = this->_undoStack.back();

    ColourType otherSide = BoardUtility::getOtherSide(this->_side);

    // Check to piece since we're going to move it back to from
    PieceType toPiece = BoardUtility::getPiece(this->_bitboards, move.to, this->_side);

    // Remove the piece from the to square
    this->removePiece(move.to, toPiece, this->_side);

    // Create the to piece in the from square
    this->createPiece(move.from, toPiece, this->_side);

    // Undo the move type
    const Castle kingSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_KING : Castle::BLACK_KING;
    const Castle queenSide = (this->_side == ColourType::WHITE) ? Castle::WHITE_QUEEN : Castle::BLACK_QUEEN;

    if (move.moveType == MoveType::CAPTURE) {
        // Create the capture piece back in the to square on the other side
        this->createPiece(move.to, undo.capturedPiece, otherSide);
    } else if (move.moveType == MoveType::EN_PASSANT) {
        // create piece at the en passant capture square
        this->createPiece(EN_PASSANT_CAPTURE_SQUARES[this->_side][BoardUtility::getFile(move.to)], PieceType::PAWN, otherSide);
    } else if (move.moveType == MoveType::KING_CASTLE) {
        this->removePiece(ROOK_TO_SQUARES[kingSide], PieceType::ROOK, this->_side);
        this->createPiece(ROOK_ORIGIN_SQUARES[kingSide], PieceType::ROOK, this->_side);
    } else if (move.moveType == MoveType::QUEEN_CASTLE) {
        // Move the queen side rook back to the origin
        this->removePiece(ROOK_TO_SQUARES[queenSide], PieceType::ROOK, this->_side);
        this->createPiece(ROOK_ORIGIN_SQUARES[queenSide], PieceType::ROOK, this->_side);
    } else if (move.isPromotionQuiet()) {
        // If promotion is quiet, the remove promotion piece which should be at the to square
        this->removePiece(move.to, move.getPromotionPiece(), this->_side);

        // Then we want to remove the piece that was just created at the from square
        this->removePiece(move.from, toPiece, this->_side);

        // Replace with pawn
        this->createPiece(move.from, PieceType::PAWN, this->_side);
    } else if (move.isPromotionCapture()) {
        // Remove the promotion piece
        this->removePiece(move.to, move.getPromotionPiece(), this->_side);

        // Place the captured piece back
        this->createPiece(move.to, undo.capturedPiece, otherSide);

        // Remove the piece we just placed at from square
        this->removePiece(move.from, toPiece, this->_side);

        // Replace with pawn
        this->createPiece(move.from, PieceType::PAWN, this->_side);
    }

    // Get back the previous state
    this->_castleRights = undo.castleRights;
    this->_enPassantSquare = undo.enPassantSquare;

    this->_undoStack.pop_back();
}

void Engine::searchRoot(int depth) {
    this->_searchResult.clear();

    int alpha = -INT_MAX;
    int beta = INT_MAX;

    MoveList moves = this->generateMoves(this->_side);

    for (int i = 0; i < moves.size; ++i) {
        Move &move = moves.moves[i];

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

    int bestScore = -INT_MAX;

    MoveList moves = this->generateMoves(this->_side);

    for (int i = 0; i < moves.size; ++i) {
        Move &move = moves.moves[i];

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

    for (int i = 0; i < captures.size; ++i) {
        Move &capture = captures.moves[i];

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
        Move &move = moves.moves[i];

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
