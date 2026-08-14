// Polls SFML keyboard state and exposes edge-triggered actions per player.
#include "Core/InputManager.h"

#include <algorithm>

namespace {
int pressedDigitSlot() {
    static const sf::Keyboard::Key keys[] = {
        sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3,
        sf::Keyboard::Key::Num4, sf::Keyboard::Key::Num5, sf::Keyboard::Key::Num6
    };
    for (int i = 0; i < 6; ++i) {
        if (sf::Keyboard::isKeyPressed(keys[i])) return i;
    }
    return -1;
}
}

void InputManager::update() {
    if (relevantKeys.empty()) {
        relevantKeys = {
            sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::W,
            sf::Keyboard::Key::S, sf::Keyboard::Key::F, sf::Keyboard::Key::H,
            sf::Keyboard::Key::Q, sf::Keyboard::Key::E,
            sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Up,
            sf::Keyboard::Key::J, sf::Keyboard::Key::L, sf::Keyboard::Key::O,
            sf::Keyboard::Key::U, sf::Keyboard::Key::I,
            sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3,
            sf::Keyboard::Key::Num4, sf::Keyboard::Key::Num5, sf::Keyboard::Key::Num6,
            sf::Keyboard::Key::Enter, sf::Keyboard::Key::P, sf::Keyboard::Key::I,
            sf::Keyboard::Key::Escape, sf::Keyboard::Key::Down
        };
    }

    previousKeys = currentKeys;
    currentKeys.clear();
    for (const auto key : relevantKeys) {
        if (sf::Keyboard::isKeyPressed(key)) currentKeys.push_back(key);
    }

    const int digitSlot = pressedDigitSlot();
    player1 = {
        down(sf::Keyboard::Key::A),
        down(sf::Keyboard::Key::D),
        pressed(sf::Keyboard::Key::W),
        down(sf::Keyboard::Key::W),
        pressed(sf::Keyboard::Key::F),
        false,
        pressed(sf::Keyboard::Key::S),
        pressed(sf::Keyboard::Key::H),
        pressed(sf::Keyboard::Key::Q),
        pressed(sf::Keyboard::Key::E),
        digitSlot
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
        pressed(sf::Keyboard::Key::U),
        pressed(sf::Keyboard::Key::I),
        -1
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
