#include <chrono>
#include <cstring>

#include "engine/Engine.hpp"

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

    this->parse(this->_INITIAL_FEN);
    // this->parse(this->_RANDOM_FEN);
}

void Engine::parse(const char *fen) {
    this->reset();

    std::vector<std::string> states = StringUtility::splitStringByWhiteSpace(fen);

    this->parseFenPosition(states[0]);
    this->parseFenSide(states[1]);
    this->parseFenCastlingRights(states[2]);
    this->parseFenEnPassantSquare(states[3]);
    this->parseFenHalfMove(states[4]);
    this->parseFenFullMove(states[5]);
}

void Engine::run() {
    this->printBoard();

    for (int i = 0; i < 50; ++i) {
        SearchResult result = this->searchRoot(this->_SEARCH_DEPTH);

        if (!result.isMoveFound) {
            continue;
        }

        Move &move = result.bestMove;

        this->makeMove(move);

        this->printBoard();

        std::cout << BoardUtility::getPositionFromSquare(move.from) << " -> " << BoardUtility::getPositionFromSquare(move.to);
        std::cout << ": " << result.bestScore << '\n';
    }
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

void Engine::parseFenCastlingRights(std::string &castlingRights) {
    this->_castlingRights = 0;

    if (castlingRights == "-") {
        return;
    }

    for (char c : castlingRights) {
        switch (c) {
        case 'K':
            this->_castlingRights |= Castle::WHITE_KING;
            break;
        case 'Q':
            this->_castlingRights |= Castle::WHITE_QUEEN;
            break;
        case 'k':
            this->_castlingRights |= Castle::BLACK_KING;
            break;
        case 'q':
            this->_castlingRights |= Castle::BLACK_QUEEN;
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

void Engine::createPiece(int rank, int file, PieceType piece, ColourType side) {
    this->createPiece(BoardUtility::getSquare(rank, file), piece, side);
}

void Engine::createPiece(int square, PieceType piece, ColourType side) {
    this->_bitboards[side][piece] |= BITBOARD_SQUARES[square];

    this->_occupancies[side] |= BITBOARD_SQUARES[square];

    this->_occupancyBoth = this->_occupancies[0] | this->_occupancies[1];
}

void Engine::removePiece(int rank, int file, PieceType piece, ColourType side) {
    this->removePiece(BoardUtility::getSquare(rank, file), piece, side);
}

void Engine::removePiece(int square, PieceType piece, ColourType side) {
    this->_bitboards[side][piece] &= ~BITBOARD_SQUARES[square];

    this->_occupancies[side] &= ~BITBOARD_SQUARES[square];

    this->_occupancyBoth = this->_occupancies[0] | this->_occupancies[1];
}

// TODO Sort moves by move type
std::vector<Move> Engine::generateMoves(ColourType side) {
    std::vector<Move> moves;

    this->generatePawnMoves(moves, side);
    this->generateKnightMoves(moves, side);
    this->generateBishopMoves(moves, side);
    this->generateRookMoves(moves, side);
    this->generateQueenMoves(moves, side);
    this->generateKingMoves(moves, side);

    return moves;
}

void Engine::generatePawnMoves(std::vector<Move> &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t pawns = this->_bitboards[side][PieceType::PAWN];

    uint64_t empty = ~this->_occupancyBoth;

    while (pawns) {
        int from = BitUtility::popLSB(pawns);

        if (Pawn::canSinglePush(from, side, empty)) {
            int to = Pawn::singlePush(from, side);

            Move move(from, to, PieceType::PAWN, side);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }

            // TODO Pawn promotion
        }

        if (Pawn::canDoublePush(from, side, empty)) {
            int to = Pawn::doublePush(from, side);

            Move move(from, to, PieceType::PAWN, side);

            move.enPassantSquare = EN_PASSANT_SQUARES[side][BoardUtility::getFile(from)];

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }

        uint64_t captureMoves = this->_PAWN_ATTACKS[side][from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::PAWN, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }

        // TODO Check if we can capture enpassant square

        // TODO Handle capture into pawn promotion
    }
}

void Engine::generateKnightMoves(std::vector<Move> &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t knights = this->_bitboards[side][PieceType::KNIGHT];

    uint64_t empty = ~this->_occupancyBoth;

    while (knights) {
        int from = BitUtility::popLSB(knights);

        uint64_t quietMoves = this->_KNIGHT_ATTACKS[from] & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            Move move(from, to, PieceType::KNIGHT, side);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }

        uint64_t captureMoves = this->_KNIGHT_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::KNIGHT, side);

            if (this->isMoveLegal(move, side)) {
                move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

                moves.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateBishopMoves(std::vector<Move> &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t bishops = this->_bitboards[side][PieceType::BISHOP];

    uint64_t empty = ~this->_occupancyBoth;

    while (bishops) {
        int from = BitUtility::popLSB(bishops);

        uint64_t attacks = Bishop::getAttacks(from, this->_occupancyBoth);

        uint64_t quietMoves = attacks & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            Move move(from, to, PieceType::BISHOP, side);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::BISHOP, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateRookMoves(std::vector<Move> &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t rooks = this->_bitboards[side][PieceType::ROOK];

    uint64_t empty = ~this->_occupancyBoth;

    while (rooks) {
        int from = BitUtility::popLSB(rooks);

        uint64_t attacks = Rook::getAttacks(from, this->_occupancyBoth);

        uint64_t quietMoves = attacks & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            Move move(from, to, PieceType::ROOK, side);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::ROOK, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateQueenMoves(std::vector<Move> &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t queens = this->_bitboards[side][PieceType::QUEEN];

    uint64_t empty = ~this->_occupancyBoth;

    while (queens) {
        int from = BitUtility::popLSB(queens);

        uint64_t attacks = Queen::getAttacks(from, this->_occupancyBoth);

        uint64_t quietMoves = attacks & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            Move move(from, to, PieceType::QUEEN, side);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::QUEEN, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateKingMoves(std::vector<Move> &moves, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t kings = this->_bitboards[side][PieceType::KING];

    uint64_t empty = ~this->_occupancyBoth;

    while (kings) {
        int from = BitUtility::popLSB(kings);

        uint64_t quietMoves = this->_KING_ATTACKS[from] & empty;

        while (quietMoves) {
            int to = BitUtility::popLSB(quietMoves);

            Move move(from, to, PieceType::KING, side);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }

        uint64_t captureMoves = this->_KING_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::KING, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                moves.push_back(std::move(move));
            }
        }
    }
}

std::vector<Move> Engine::generateCaptures(ColourType side) {
    std::vector<Move> captures;

    this->generatePawnCaptures(captures, side);
    this->generateKnightCaptures(captures, side);
    this->generateBishopCaptures(captures, side);
    this->generateRookCaptures(captures, side);
    this->generateQueenCaptures(captures, side);
    this->generateKingCaptures(captures, side);

    return captures;
}

void Engine::generatePawnCaptures(std::vector<Move> &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t pawns = this->_bitboards[side][PieceType::PAWN];

    uint64_t empty = ~this->_occupancyBoth;

    while (pawns) {
        int from = BitUtility::popLSB(pawns);

        uint64_t captureMoves = this->_PAWN_ATTACKS[side][from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::PAWN, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                captures.push_back(std::move(move));
            }
        }

        // TODO Check if we can capture enpassant square

        // TODO Handle pawn capture into promotion
    }
}

void Engine::generateKnightCaptures(std::vector<Move> &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t knights = this->_bitboards[side][PieceType::KNIGHT];

    uint64_t empty = ~this->_occupancyBoth;

    while (knights) {
        int from = BitUtility::popLSB(knights);

        uint64_t captureMoves = this->_KNIGHT_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::KNIGHT, side);

            if (this->isMoveLegal(move, side)) {
                move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

                captures.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateBishopCaptures(std::vector<Move> &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t bishops = this->_bitboards[side][PieceType::BISHOP];

    uint64_t empty = ~this->_occupancyBoth;

    while (bishops) {
        int from = BitUtility::popLSB(bishops);

        uint64_t attacks = Bishop::getAttacks(from, this->_occupancyBoth);

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::BISHOP, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                captures.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateRookCaptures(std::vector<Move> &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t rooks = this->_bitboards[side][PieceType::ROOK];

    uint64_t empty = ~this->_occupancyBoth;

    while (rooks) {
        int from = BitUtility::popLSB(rooks);

        uint64_t attacks = Rook::getAttacks(from, this->_occupancyBoth);

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::ROOK, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                captures.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateQueenCaptures(std::vector<Move> &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t queens = this->_bitboards[side][PieceType::QUEEN];

    uint64_t empty = ~this->_occupancyBoth;

    while (queens) {
        int from = BitUtility::popLSB(queens);

        uint64_t attacks = Queen::getAttacks(from, this->_occupancyBoth);

        uint64_t captureMoves = attacks & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::QUEEN, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                captures.push_back(std::move(move));
            }
        }
    }
}

void Engine::generateKingCaptures(std::vector<Move> &captures, ColourType side) {
    ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t kings = this->_bitboards[side][PieceType::KING];

    uint64_t empty = ~this->_occupancyBoth;

    while (kings) {
        int from = BitUtility::popLSB(kings);

        uint64_t captureMoves = this->_KING_ATTACKS[from] & this->_occupancies[otherSide];

        while (captureMoves) {
            int to = BitUtility::popLSB(captureMoves);

            Move move(from, to, PieceType::KING, side);

            move.capturedPiece = BoardUtility::getPiece(this->_bitboards, to, otherSide);

            if (this->isMoveLegal(move, side)) {
                captures.push_back(std::move(move));
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

int Engine::getKingSquare(ColourType side) {
    return BitUtility::getLSBIndex(this->_bitboards[side][PieceType::KING]);
}

void Engine::makeMove(Move &move) {
    ColourType otherSide = BoardUtility::getOtherSide(move.colour);

    this->removePiece(move.from, move.piece, move.colour);

    this->createPiece(move.to, move.piece, move.colour);

    if (move.capturedPiece != PieceType::EMPTY) {
        this->removePiece(move.to, move.capturedPiece, otherSide);
    }

    this->switchSide();
}

void Engine::unmakeMove(Move &move) {
    ColourType otherSide = BoardUtility::getOtherSide(move.colour);

    this->removePiece(move.to, move.piece, move.colour);

    this->createPiece(move.from, move.piece, move.colour);

    if (move.capturedPiece != PieceType::EMPTY) {
        this->createPiece(move.to, move.capturedPiece, otherSide);
    }

    this->switchSide();
}

SearchResult Engine::searchRoot(int depth) {
    SearchResult result;

    int alpha = -INT_MAX;
    int beta = INT_MAX;

    std::vector<Move> moves = this->generateMoves(this->_side);

    for (Move &move : moves) {
        this->makeMove(move);

        int score = -this->search(-beta, -alpha, depth - 1);

        this->unmakeMove(move);

        if (score > result.bestScore) {
            result.bestScore = score;

            result.bestMove = move;

            result.isMoveFound = true;

            if (score > alpha) {
                alpha = score;
            }
        }

        if (score >= beta) {
            break;
        }
    }

    return result;
}

int Engine::search(int alpha, int beta, int depth) {
    if (depth == 0) {
        return this->quiescence(alpha, beta);
    }

    // TODO Implement null move pruning- condition
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

    std::vector<Move> moves = this->generateMoves(this->_side);

    for (Move &move : moves) {
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

    std::vector<Move> captures = this->generateCaptures(this->_side);

    for (Move &capture : captures) {
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

// TODO implement mobility score
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

    std::vector<Move> moves = this->generateMoves(this->_side);

    for (Move &move : moves) {
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
    this->_castlingRights = this->_INITIAL_CASTLING_RIGHTS;
    this->_side = ColourType::WHITE;
    this->_enPassantSquare = -1;
}

} // namespace engine
