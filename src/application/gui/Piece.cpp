#include "application/gui/Piece.hpp"

using namespace engine::board;

namespace application::gui {

Piece::Piece() : _piece(PieceType::EMPTY), _colour(ColourType::NONE) {
}

Piece::Piece(PieceType piece, ColourType colour) : _piece(piece), _colour(colour) {
}

} // namespace application::gui
