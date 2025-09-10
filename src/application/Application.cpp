#include <SFML/Window/Mouse.hpp>

#include "application/Application.hpp"

#include "application/manager/TextureManager.hpp"

#include "engine/board/Fen.hpp"

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
    while (this->_window.isOpen()) {
        while (const std::optional event = this->_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                this->_window.close();
            }

            if (event->is<sf::Event::MouseButtonPressed>()) {
                const auto &mouseEvent = event->getIf<sf::Event::MouseButtonPressed>();

                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    this->_chess.move(this->_window, this->_engine, mouseEvent->position);
                }
            }
        }

        this->update();
        this->render();
    }
}

void Application::debug() {
    this->_engine.parse(POSITIONS[0]);

    this->_engine.printBoard();

    for (int depth = 1; depth <= 4; ++depth) {
        this->_engine.runPerft(depth);
    }
}

void Application::update() {
    this->_chess.update(this->_window, this->_engine);
}

void Application::render() {
    this->_window.clear(BACKGROUND_COLOUR);

    this->_chess.render(this->_window);

    this->_window.display();
}

} // namespace application
