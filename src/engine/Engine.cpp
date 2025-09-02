#include "engine/Engine.hpp"

using namespace engine::board;

namespace engine {

Engine::Engine() {
    // std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR - - - - -";
    std::string fen = "r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2";

    this->_board = Board(fen);
}

Board &Engine::getBoard() {
    return this->_board;
}

} // namespace engine