#include <SFML/Graphics.hpp>

#include "application/manager/MouseManager.hpp"

namespace application::manager {
MouseManager::MouseManager() = default;

MouseManager::~MouseManager() = default;

MouseManager &MouseManager::getInstance() {
    static MouseManager instance;

    return instance;
}

bool MouseManager::isClick() {
    if (!this->_isClicking && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        this->_isClicking = true;

        return false;
    }

    if (this->_isClicking && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        this->_isClicking = false;

        return true;
    }

    return false;
}

void MouseManager::initialise() {
}

} // namespace application::manager
