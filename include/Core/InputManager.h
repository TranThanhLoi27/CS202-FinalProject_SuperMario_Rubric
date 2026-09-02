#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <array>
#include <cstddef>
#include <string>
#include <unordered_map>

enum class Action {
    MoveLeft,
    MoveRight,
    Jump,
    Attack,
    Dash,
    Dodge,
    UseItem,
    PrevSlot,
    NextSlot,
    Slot1,
    Slot2,
    Slot3,
    Slot4,
    Slot5,
    Slot6,
    Pause
};

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
    using ActionBindings = std::unordered_map<Action, sf::Keyboard::Key>;

    InputManager();
    void update();
    const InputState& getPlayer1Input() const;
    const InputState& getPlayer2Input() const;

    bool isActionActive(Action action) const;
    bool isActionActive(Action action, std::size_t playerIndex) const;
    bool isActionPressed(Action action, std::size_t playerIndex = 0) const;
    bool rebindKey(Action action, sf::Keyboard::Key newKey);
    bool rebindKey(Action action, sf::Keyboard::Key newKey, std::size_t playerIndex);
    sf::Keyboard::Key getBinding(Action action, std::size_t playerIndex = 0) const;
    const ActionBindings& getBindings(std::size_t playerIndex = 0) const;

    static const std::array<Action, 16>& actions();
    static std::string actionToString(Action action);
    static std::string keyToString(sf::Keyboard::Key key);

    // Raw-key helpers remain available for non-rebindable system shortcuts.
    bool pressed(sf::Keyboard::Key key) const;
    bool down(sf::Keyboard::Key key) const;

private:
    void initializeDefaultBindings();
    InputState makeInputState(std::size_t playerIndex) const;

    std::array<ActionBindings, 2> keyBindings;
    std::unordered_map<sf::Keyboard::Key, bool> currentKeyStates;
    std::unordered_map<sf::Keyboard::Key, bool> previousKeyStates;
    InputState player1;
    InputState player2;
};
