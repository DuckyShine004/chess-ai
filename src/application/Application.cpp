#include <SFML/Window/Mouse.hpp>
#include <iostream>

#include "application/Application.hpp"

#include "application/manager/TextureManager.hpp"

#include "engine/board/Colour.hpp"

using namespace engine::board;

using namespace application::manager;

namespace application {

Application::Application() {
    sf::VideoMode mode(sf::Vector2u(1440, 900));

    this->_window = sf::RenderWindow(mode, "Chess");
}

void Application::initialise() {
    sf::ContextSettings settings;

    settings.antiAliasingLevel = 8;

    TextureManager::getInstance().initialise();
}

void Application::run() {
    // this->_engine.run();

    // for (int depth = 1; depth <= 5; ++depth) {
    //     this->_engine.runPerft(depth);
    // }

    while (this->_window.isOpen()) {
        while (const std::optional event = this->_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                this->_window.close();
            }

            // if (event->is<sf::Event::MouseButtonPressed>()) {
            //     const auto& mouseEvent = event->get<sf::Event::MouseButtonPressed>();
            // }
        }

        this->update();
        this->render();
    }
}

void Application::update() {
    // Move move = this->_chess.getMove(engine);

    // if (move) {
    // this->_chess.move(move);
    // }
    // this->_chess.move(this->_engine);
    // ColourType side = this->_engine.getSide();

    // if (side == ColourType::WHITE) {
    //     this->_chess.move(side);
    // } else {
    //     this->_engine.run();
    // }
    this->_chess.update(this->_window, this->_engine);
}

void Application::render() {
    this->_window.clear(sf::Color(22, 21, 18));

    this->_chess.render(this->_window);

    this->_window.display();
}

} // namespace application
