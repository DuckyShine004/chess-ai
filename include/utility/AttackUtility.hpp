#pragma once

#include <cstdint>

#include "engine/piece/Pawn.hpp"
#include "engine/piece/Knight.hpp"
#include "engine/piece/Bishop.hpp"
#include "engine/piece/Rook.hpp"
#include "engine/piece/King.hpp"

namespace utility::AttackUtility {

[[nodiscard]] inline uint64_t getAttackersToSquare(int square, uint64_t bitboards[2][6], uint64_t occupancy, engine::board::ColourType side);

[[nodiscard]] inline bool getLeastValuableAttacker(int square, uint64_t bitboards[2][6], uint64_t occupancy, engine::board::ColourType side, int &from, engine::board::PieceType &piece);

[[nodiscard]] inline uint64_t getAttackersToSquare(int square, uint64_t bitboards[2][6], uint64_t occupancy, engine::board::ColourType side) {
    engine::board::ColourType otherSide = BoardUtility::getOtherSide(side);

    uint64_t attackers = 0ULL;

    attackers |= (engine::piece::Pawn::ATTACKS[otherSide][square] & bitboards[side][engine::board::PieceType::PAWN]);

    attackers |= (engine::piece::Knight::ATTACKS[square] & bitboards[side][engine::board::PieceType::KNIGHT]);

    attackers |= (engine::piece::King::ATTACKS[square] & bitboards[side][engine::board::PieceType::KING]);

    uint64_t bishopAttacks = engine::piece::Bishop::getAttacks(square, occupancy);

    attackers |= (bishopAttacks & (bitboards[side][engine::board::PieceType::BISHOP] | bitboards[side][engine::board::PieceType::QUEEN]));

    uint64_t rookAttacks = engine::piece::Rook::getAttacks(square, occupancy);

    attackers |= (rookAttacks & (bitboards[side][engine::board::PieceType::ROOK] | bitboards[side][engine::board::PieceType::QUEEN]));

    return attackers;
}

[[nodiscard]] inline bool getLeastValuableAttacker(int square, uint64_t bitboards[2][6], uint64_t occupancy, engine::board::ColourType side, int &from, engine::board::PieceType &piece) {
    uint64_t attackers = getAttackersToSquare(square, bitboards, occupancy, side);

    if (!attackers) {
        return false;
    }

    uint64_t attackerPieces = attackers & bitboards[side][engine::board::PieceType::PAWN];

    if (attackerPieces) {
        from = BitUtility::getLSBIndex(attackerPieces);

        piece = engine::board::PieceType::PAWN;

        return true;
    }

    attackerPieces = attackers & bitboards[side][engine::board::PieceType::KNIGHT];

    if (attackerPieces) {
        from = BitUtility::getLSBIndex(attackerPieces);

        piece = engine::board::PieceType::KNIGHT;

        return true;
    }

    attackerPieces = attackers & bitboards[side][engine::board::PieceType::BISHOP];

    if (attackerPieces) {
        from = BitUtility::getLSBIndex(attackerPieces);

        piece = engine::board::PieceType::BISHOP;

        return true;
    }

    attackerPieces = attackers & bitboards[side][engine::board::PieceType::ROOK];

    if (attackerPieces) {
        from = BitUtility::getLSBIndex(attackerPieces);

        piece = engine::board::PieceType::ROOK;

        return true;
    }

    attackerPieces = attackers & bitboards[side][engine::board::PieceType::QUEEN];

    if (attackerPieces) {
        from = BitUtility::getLSBIndex(attackerPieces);

        piece = engine::board::PieceType::QUEEN;

        return true;
    }

    attackerPieces = attackers & bitboards[side][engine::board::PieceType::KING];

    if (attackerPieces) {
        from = BitUtility::getLSBIndex(attackerPieces);

        piece = engine::board::PieceType::KING;

        return true;
    }

    return false;
}

} // namespace utility::AttackUtility
