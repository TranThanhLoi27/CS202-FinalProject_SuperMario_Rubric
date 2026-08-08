// Polls SFML keyboard state and exposes edge-triggered actions per player.
#include "Core/InputManager.h"

#include <algorithm>

void InputManager::update() {
    if (relevantKeys.empty()) {
        relevantKeys = {
            sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::W,
            sf::Keyboard::Key::S, sf::Keyboard::Key::F, sf::Keyboard::Key::H, sf::Keyboard::Key::R,
            sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Up,
            sf::Keyboard::Key::J, sf::Keyboard::Key::L, sf::Keyboard::Key::O,
            sf::Keyboard::Key::Enter, sf::Keyboard::Key::P, sf::Keyboard::Key::I,
            sf::Keyboard::Key::Escape, sf::Keyboard::Key::Down
        };
    }

    previousKeys = currentKeys;
    currentKeys.clear();
    for (const auto key : relevantKeys) {
        if (sf::Keyboard::isKeyPressed(key)) currentKeys.push_back(key);
    }

    player1 = {
        down(sf::Keyboard::Key::A),
        down(sf::Keyboard::Key::D),
        pressed(sf::Keyboard::Key::W),
        down(sf::Keyboard::Key::W),
        pressed(sf::Keyboard::Key::F),
        false,
        pressed(sf::Keyboard::Key::S),
        pressed(sf::Keyboard::Key::H),
        pressed(sf::Keyboard::Key::R)
    };
    player2 = {
        down(sf::Keyboard::Key::Left),
        down(sf::Keyboard::Key::Right),
        pressed(sf::Keyboard::Key::Up),
        down(sf::Keyboard::Key::Up),
        pressed(sf::Keyboard::Key::J),
        false,
        pressed(sf::Keyboard::Key::Down),
        pressed(sf::Keyboard::Key::L),
        pressed(sf::Keyboard::Key::O)
    };
}

const InputState& InputManager::getPlayer1Input() const {
    return player1;
}

const InputState& InputManager::getPlayer2Input() const {
    return player2;
}

bool InputManager::pressed(sf::Keyboard::Key key) const {
    return down(key) && std::find(previousKeys.begin(), previousKeys.end(), key) == previousKeys.end();
}

bool InputManager::down(sf::Keyboard::Key key) const {
    return std::find(currentKeys.begin(), currentKeys.end(), key) != currentKeys.end();
}
