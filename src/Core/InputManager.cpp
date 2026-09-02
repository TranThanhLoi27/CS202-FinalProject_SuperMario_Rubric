// Polls SFML keyboard state and exposes edge-triggered actions per player.
#include "Core/InputManager.h"

#include <stdexcept>

namespace {
constexpr std::size_t kPlayerCount = 2;

int keyValue(sf::Keyboard::Key key) {
    return static_cast<int>(key);
}

bool isBetween(sf::Keyboard::Key key, sf::Keyboard::Key first, sf::Keyboard::Key last) {
    return keyValue(key) >= keyValue(first) && keyValue(key) <= keyValue(last);
}
}

InputManager::InputManager() {
    initializeDefaultBindings();
}

void InputManager::initializeDefaultBindings() {
    keyBindings[0] = {
        {Action::MoveLeft, sf::Keyboard::Key::A},
        {Action::MoveRight, sf::Keyboard::Key::D},
        {Action::Jump, sf::Keyboard::Key::W},
        {Action::Attack, sf::Keyboard::Key::F},
        {Action::Dash, sf::Keyboard::Key::G},
        {Action::Dodge, sf::Keyboard::Key::S},
        {Action::UseItem, sf::Keyboard::Key::H},
        {Action::PrevSlot, sf::Keyboard::Key::Q},
        {Action::NextSlot, sf::Keyboard::Key::E},
        {Action::Slot1, sf::Keyboard::Key::Num1},
        {Action::Slot2, sf::Keyboard::Key::Num2},
        {Action::Slot3, sf::Keyboard::Key::Num3},
        {Action::Slot4, sf::Keyboard::Key::Num4},
        {Action::Slot5, sf::Keyboard::Key::Num5},
        {Action::Slot6, sf::Keyboard::Key::Num6},
        {Action::Pause, sf::Keyboard::Key::P}
    };
    keyBindings[1] = {
        {Action::MoveLeft, sf::Keyboard::Key::Left},
        {Action::MoveRight, sf::Keyboard::Key::Right},
        {Action::Jump, sf::Keyboard::Key::Up},
        {Action::Attack, sf::Keyboard::Key::J},
        {Action::Dash, sf::Keyboard::Key::K},
        {Action::Dodge, sf::Keyboard::Key::Down},
        {Action::UseItem, sf::Keyboard::Key::L},
        {Action::PrevSlot, sf::Keyboard::Key::U},
        {Action::NextSlot, sf::Keyboard::Key::I},
        {Action::Slot1, sf::Keyboard::Key::Unknown},
        {Action::Slot2, sf::Keyboard::Key::Unknown},
        {Action::Slot3, sf::Keyboard::Key::Unknown},
        {Action::Slot4, sf::Keyboard::Key::Unknown},
        {Action::Slot5, sf::Keyboard::Key::Unknown},
        {Action::Slot6, sf::Keyboard::Key::Unknown},
        {Action::Pause, sf::Keyboard::Key::Unknown}
    };
}

void InputManager::update() {
    previousKeyStates = currentKeyStates;
    currentKeyStates.clear();
    for (const auto& playerBindings : keyBindings) {
        for (const auto& [action, key] : playerBindings) {
            (void)action;
            if (key != sf::Keyboard::Key::Unknown) {
                currentKeyStates[key] = sf::Keyboard::isKeyPressed(key);
            }
        }
    }

    player1 = makeInputState(0);
    player2 = makeInputState(1);
}

InputState InputManager::makeInputState(std::size_t playerIndex) const {
    InputState state;
    state.moveLeft = isActionActive(Action::MoveLeft, playerIndex);
    state.moveRight = isActionActive(Action::MoveRight, playerIndex);
    state.jump = isActionPressed(Action::Jump, playerIndex);
    state.jumpHeld = isActionActive(Action::Jump, playerIndex);
    state.attack = isActionPressed(Action::Attack, playerIndex);
    state.dash = isActionPressed(Action::Dash, playerIndex);
    state.dodge = isActionPressed(Action::Dodge, playerIndex);
    state.useItem = isActionPressed(Action::UseItem, playerIndex);
    state.slotPrev = isActionPressed(Action::PrevSlot, playerIndex);
    state.slotNext = isActionPressed(Action::NextSlot, playerIndex);

    constexpr std::array<Action, 6> slotActions = {
        Action::Slot1, Action::Slot2, Action::Slot3,
        Action::Slot4, Action::Slot5, Action::Slot6
    };
    for (std::size_t i = 0; i < slotActions.size(); ++i) {
        if (isActionPressed(slotActions[i], playerIndex)) {
            state.slotSelect = static_cast<int>(i);
            break;
        }
    }
    return state;
}

const InputState& InputManager::getPlayer1Input() const {
    return player1;
}

const InputState& InputManager::getPlayer2Input() const {
    return player2;
}

bool InputManager::isActionActive(Action action) const {
    return isActionActive(action, 0);
}

bool InputManager::isActionActive(Action action, std::size_t playerIndex) const {
    const sf::Keyboard::Key key = getBinding(action, playerIndex);
    return key != sf::Keyboard::Key::Unknown && sf::Keyboard::isKeyPressed(key);
}

bool InputManager::isActionPressed(Action action, std::size_t playerIndex) const {
    return pressed(getBinding(action, playerIndex));
}

bool InputManager::rebindKey(Action action, sf::Keyboard::Key newKey) {
    return rebindKey(action, newKey, 0);
}

bool InputManager::rebindKey(Action action, sf::Keyboard::Key newKey, std::size_t playerIndex) {
    if (playerIndex >= kPlayerCount || newKey == sf::Keyboard::Key::Unknown) return false;
    for (std::size_t otherPlayer = 0; otherPlayer < keyBindings.size(); ++otherPlayer) {
        for (const auto& [boundAction, boundKey] : keyBindings[otherPlayer]) {
            const bool sameBinding = otherPlayer == playerIndex && boundAction == action;
            if (!sameBinding && boundKey == newKey) return false;
        }
    }
    keyBindings[playerIndex][action] = newKey;
    currentKeyStates[newKey] = sf::Keyboard::isKeyPressed(newKey);
    previousKeyStates[newKey] = currentKeyStates[newKey];
    return true;
}

sf::Keyboard::Key InputManager::getBinding(Action action, std::size_t playerIndex) const {
    if (playerIndex >= kPlayerCount) return sf::Keyboard::Key::Unknown;
    const auto& bindings = keyBindings[playerIndex];
    const auto it = bindings.find(action);
    return it == bindings.end() ? sf::Keyboard::Key::Unknown : it->second;
}

const InputManager::ActionBindings& InputManager::getBindings(std::size_t playerIndex) const {
    if (playerIndex >= kPlayerCount) throw std::out_of_range("Invalid player binding index");
    return keyBindings[playerIndex];
}

const std::array<Action, 16>& InputManager::actions() {
    static const std::array<Action, 16> values = {
        Action::MoveLeft, Action::MoveRight, Action::Jump, Action::Attack,
        Action::Dash, Action::Dodge, Action::UseItem, Action::PrevSlot,
        Action::NextSlot, Action::Slot1, Action::Slot2, Action::Slot3,
        Action::Slot4, Action::Slot5, Action::Slot6, Action::Pause
    };
    return values;
}

std::string InputManager::actionToString(Action action) {
    static const std::unordered_map<Action, std::string> names = {
        {Action::MoveLeft, "Move Left"}, {Action::MoveRight, "Move Right"},
        {Action::Jump, "Jump"}, {Action::Attack, "Attack"}, {Action::Dash, "Dash"},
        {Action::Dodge, "Dodge"}, {Action::UseItem, "Use Item"},
        {Action::PrevSlot, "Previous Slot"}, {Action::NextSlot, "Next Slot"},
        {Action::Slot1, "Select Slot 1"}, {Action::Slot2, "Select Slot 2"},
        {Action::Slot3, "Select Slot 3"}, {Action::Slot4, "Select Slot 4"},
        {Action::Slot5, "Select Slot 5"}, {Action::Slot6, "Select Slot 6"},
        {Action::Pause, "Pause"}
    };
    const auto it = names.find(action);
    return it == names.end() ? "Unknown Action" : it->second;
}

std::string InputManager::keyToString(sf::Keyboard::Key key) {
    if (isBetween(key, sf::Keyboard::Key::A, sf::Keyboard::Key::Z)) {
        return std::string(1, static_cast<char>('A' + keyValue(key) - keyValue(sf::Keyboard::Key::A)));
    }
    if (isBetween(key, sf::Keyboard::Key::Num0, sf::Keyboard::Key::Num9)) {
        return std::string(1, static_cast<char>('0' + keyValue(key) - keyValue(sf::Keyboard::Key::Num0)));
    }
    if (isBetween(key, sf::Keyboard::Key::Numpad0, sf::Keyboard::Key::Numpad9)) {
        return "NUMPAD_" + std::to_string(keyValue(key) - keyValue(sf::Keyboard::Key::Numpad0));
    }
    if (isBetween(key, sf::Keyboard::Key::F1, sf::Keyboard::Key::F15)) {
        return "F" + std::to_string(keyValue(key) - keyValue(sf::Keyboard::Key::F1) + 1);
    }

    static const std::unordered_map<sf::Keyboard::Key, std::string> names = {
        {sf::Keyboard::Key::Unknown, "UNBOUND"}, {sf::Keyboard::Key::Escape, "ESCAPE"},
        {sf::Keyboard::Key::Space, "SPACE"}, {sf::Keyboard::Key::Enter, "ENTER"},
        {sf::Keyboard::Key::Tab, "TAB"}, {sf::Keyboard::Key::Backspace, "BACKSPACE"},
        {sf::Keyboard::Key::Left, "LEFT_ARROW"}, {sf::Keyboard::Key::Right, "RIGHT_ARROW"},
        {sf::Keyboard::Key::Up, "UP_ARROW"}, {sf::Keyboard::Key::Down, "DOWN_ARROW"},
        {sf::Keyboard::Key::LShift, "LEFT_SHIFT"}, {sf::Keyboard::Key::RShift, "RIGHT_SHIFT"},
        {sf::Keyboard::Key::LControl, "LEFT_CTRL"}, {sf::Keyboard::Key::RControl, "RIGHT_CTRL"},
        {sf::Keyboard::Key::LAlt, "LEFT_ALT"}, {sf::Keyboard::Key::RAlt, "RIGHT_ALT"},
        {sf::Keyboard::Key::PageUp, "PAGE_UP"}, {sf::Keyboard::Key::PageDown, "PAGE_DOWN"},
        {sf::Keyboard::Key::Home, "HOME"}, {sf::Keyboard::Key::End, "END"},
        {sf::Keyboard::Key::Insert, "INSERT"}, {sf::Keyboard::Key::Delete, "DELETE"},
        {sf::Keyboard::Key::Pause, "PAUSE"}
    };
    const auto it = names.find(key);
    return it == names.end() ? "KEY_" + std::to_string(keyValue(key)) : it->second;
}

bool InputManager::pressed(sf::Keyboard::Key key) const {
    if (key == sf::Keyboard::Key::Unknown) return false;
    const auto current = currentKeyStates.find(key);
    if (current == currentKeyStates.end() || !current->second) return false;
    const auto previous = previousKeyStates.find(key);
    return previous == previousKeyStates.end() || !previous->second;
}

bool InputManager::down(sf::Keyboard::Key key) const {
    if (key == sf::Keyboard::Key::Unknown) return false;
    const auto it = currentKeyStates.find(key);
    return it != currentKeyStates.end() && it->second;
}
