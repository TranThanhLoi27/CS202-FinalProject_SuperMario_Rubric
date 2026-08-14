#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <vector>

struct InputState {
    bool moveLeft = false;
    bool moveRight = false;
    bool jump = false;
    bool jumpHeld = false;
    bool attack = false;
    bool dash = false;
    bool dodge = false;
    bool useItem = false;
    bool slotPrev = false;
    bool slotNext = false;
    int slotSelect = -1;
};

class InputManager {
public:
    void update();
    const InputState& getPlayer1Input() const;
    const InputState& getPlayer2Input() const;
    bool pressed(sf::Keyboard::Key key) const;
    bool down(sf::Keyboard::Key key) const;

private:
    std::vector<sf::Keyboard::Key> relevantKeys;
    std::vector<sf::Keyboard::Key> currentKeys;
    std::vector<sf::Keyboard::Key> previousKeys;
    InputState player1;
    InputState player2;
};
