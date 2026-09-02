// Polls SFML keyboard state and exposes edge-triggered actions per player.
#include "Core/InputManager.h"

#include <algorithm>

namespace {
// Returns the zero-based inventory slot selected by a pressed number key.
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

InputManager::InputManager() {
    // Default Player 1 keybinds
    p1Keys[MoveLeft]  = sf::Keyboard::Key::A;
    p1Keys[MoveRight] = sf::Keyboard::Key::D;
    p1Keys[Jump]      = sf::Keyboard::Key::W;
    p1Keys[Attack]    = sf::Keyboard::Key::F;
    p1Keys[Dodge]     = sf::Keyboard::Key::S;
    p1Keys[UseItem]   = sf::Keyboard::Key::H;
    p1Keys[SlotPrev]  = sf::Keyboard::Key::Q;
    p1Keys[SlotNext]  = sf::Keyboard::Key::E;
    p1Keys[MenuUp]    = sf::Keyboard::Key::Up;
    p1Keys[MenuDown]  = sf::Keyboard::Key::Down;
    p1Keys[MenuLeft]  = sf::Keyboard::Key::Left;
    p1Keys[MenuRight] = sf::Keyboard::Key::Right;
    p1Keys[MenuConfirm] = sf::Keyboard::Key::Enter;
    p1Keys[MenuBack]  = sf::Keyboard::Key::Escape;
    p1Keys[Pause]     = sf::Keyboard::Key::P;
    p1Keys[Info]      = sf::Keyboard::Key::I;

    // Default Player 2 keybinds
    p2Keys[MoveLeft]  = sf::Keyboard::Key::Left;
    p2Keys[MoveRight] = sf::Keyboard::Key::Right;
    p2Keys[Jump]      = sf::Keyboard::Key::Up;
    p2Keys[Attack]    = sf::Keyboard::Key::J;
    p2Keys[Dodge]     = sf::Keyboard::Key::Down;
    p2Keys[UseItem]   = sf::Keyboard::Key::L;
    p2Keys[SlotPrev]  = sf::Keyboard::Key::U;
    p2Keys[SlotNext]  = sf::Keyboard::Key::I;
    p2Keys[MenuUp]    = sf::Keyboard::Key::Numpad8;
    p2Keys[MenuDown]  = sf::Keyboard::Key::Numpad2;
    p2Keys[MenuLeft]  = sf::Keyboard::Key::Numpad4;
    p2Keys[MenuRight] = sf::Keyboard::Key::Numpad6;
    p2Keys[MenuConfirm] = sf::Keyboard::Key::Numpad0;
    p2Keys[MenuBack]  = sf::Keyboard::Key::Numpad7;
    p2Keys[Pause]     = sf::Keyboard::Key::K;
    p2Keys[Info]      = sf::Keyboard::Key::O;

    // Initialize mouse tracking
    mousePosition = {0, 0};
    mouseWheelDelta = 0.0f;
}

void InputManager::update() {
    // Update mouse tracking using screen coordinates as fallback
    mousePosition = sf::Mouse::getPosition();
    previousMouseButtons = currentMouseButtons;
    currentMouseButtons.clear();
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) currentMouseButtons.push_back(sf::Mouse::Button::Left);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) currentMouseButtons.push_back(sf::Mouse::Button::Right);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) currentMouseButtons.push_back(sf::Mouse::Button::Middle);
    
    // Process input states
    static const sf::Keyboard::Key allKeys[] = {
        sf::Keyboard::Key::A, sf::Keyboard::Key::B, sf::Keyboard::Key::C, sf::Keyboard::Key::D,
        sf::Keyboard::Key::E, sf::Keyboard::Key::F, sf::Keyboard::Key::G, sf::Keyboard::Key::H,
        sf::Keyboard::Key::I, sf::Keyboard::Key::J, sf::Keyboard::Key::K, sf::Keyboard::Key::L,
        sf::Keyboard::Key::M, sf::Keyboard::Key::N, sf::Keyboard::Key::O, sf::Keyboard::Key::P,
        sf::Keyboard::Key::Q, sf::Keyboard::Key::R, sf::Keyboard::Key::S, sf::Keyboard::Key::T,
        sf::Keyboard::Key::U, sf::Keyboard::Key::V, sf::Keyboard::Key::W, sf::Keyboard::Key::X,
        sf::Keyboard::Key::Y, sf::Keyboard::Key::Z,
        sf::Keyboard::Key::Num0, sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2,
        sf::Keyboard::Key::Num3, sf::Keyboard::Key::Num4, sf::Keyboard::Key::Num5,
        sf::Keyboard::Key::Num6, sf::Keyboard::Key::Num7, sf::Keyboard::Key::Num8, sf::Keyboard::Key::Num9,
        sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Up, sf::Keyboard::Key::Down,
        sf::Keyboard::Key::Space, sf::Keyboard::Key::Enter, sf::Keyboard::Key::Tab,
        sf::Keyboard::Key::LControl, sf::Keyboard::Key::LShift, sf::Keyboard::Key::LAlt,
        sf::Keyboard::Key::RControl, sf::Keyboard::Key::RShift, sf::Keyboard::Key::RAlt,
        sf::Keyboard::Key::Numpad0, sf::Keyboard::Key::Numpad1, sf::Keyboard::Key::Numpad2,
        sf::Keyboard::Key::Numpad3, sf::Keyboard::Key::Numpad4, sf::Keyboard::Key::Numpad5,
        sf::Keyboard::Key::Numpad6, sf::Keyboard::Key::Numpad7, sf::Keyboard::Key::Numpad8,
        sf::Keyboard::Key::Numpad9
    };

    relevantKeys.clear();
    for (auto k : allKeys) {
        relevantKeys.push_back(k);
    }
    for (int i = 0; i < ActionCount; ++i) {
        relevantKeys.push_back(p1Keys[i]);
        relevantKeys.push_back(p2Keys[i]);
    }
    std::sort(relevantKeys.begin(), relevantKeys.end());
    relevantKeys.erase(std::unique(relevantKeys.begin(), relevantKeys.end()), relevantKeys.end());

    previousKeys = currentKeys;
    currentKeys.clear();
    for (const auto key : relevantKeys) {
        if (sf::Keyboard::isKeyPressed(key)) currentKeys.push_back(key);
    }

    const int digitSlot = pressedDigitSlot();
    player1 = {
        down(p1Keys[MoveLeft]),
        down(p1Keys[MoveRight]),
        pressed(p1Keys[Jump]),
        down(p1Keys[Jump]),
        pressed(p1Keys[Attack]),
        false,
        pressed(p1Keys[Dodge]),
        pressed(p1Keys[UseItem]),
        pressed(p1Keys[SlotPrev]),
        pressed(p1Keys[SlotNext]),
        digitSlot,
        pressed(p1Keys[MenuUp]),
        pressed(p1Keys[MenuDown]),
        pressed(p1Keys[MenuLeft]),
        pressed(p1Keys[MenuRight]),
        pressed(p1Keys[MenuConfirm]),
        pressed(p1Keys[MenuBack]),
        pressed(p1Keys[Pause]),
        pressed(p1Keys[Info])
    };
    player2 = {
        down(p2Keys[MoveLeft]),
        down(p2Keys[MoveRight]),
        pressed(p2Keys[Jump]),
        down(p2Keys[Jump]),
        pressed(p2Keys[Attack]),
        false,
        pressed(p2Keys[Dodge]),
        pressed(p2Keys[UseItem]),
        pressed(p2Keys[SlotPrev]),
        pressed(p2Keys[SlotNext]),
        -1,
        pressed(p2Keys[MenuUp]),
        pressed(p2Keys[MenuDown]),
        pressed(p2Keys[MenuLeft]),
        pressed(p2Keys[MenuRight]),
        pressed(p2Keys[MenuConfirm]),
        pressed(p2Keys[MenuBack]),
        pressed(p2Keys[Pause]),
        pressed(p2Keys[Info])
    };
}

void InputManager::update(const sf::WindowBase& window) {
    update();
    mousePosition = sf::Mouse::getPosition(window);
}

const InputState& InputManager::getPlayer1Input() const {
    return player1;
}

const InputState& InputManager::getPlayer2Input() const {
    return player2;
}

bool InputManager::pressed(sf::Keyboard::Key key) const {
    if (key == sf::Keyboard::Key::Unknown) return false;
    return down(key) && std::find(previousKeys.begin(), previousKeys.end(), key) == previousKeys.end();
}

bool InputManager::down(sf::Keyboard::Key key) const {
    if (key == sf::Keyboard::Key::Unknown) return false;
    return std::find(currentKeys.begin(), currentKeys.end(), key) != currentKeys.end();
}

sf::Keyboard::Key InputManager::getKey(int playerIndex, int actionIndex) const {
    if (actionIndex < 0 || actionIndex >= ActionCount) return sf::Keyboard::Key::Unknown;
    return (playerIndex == 0) ? p1Keys[actionIndex] : p2Keys[actionIndex];
}

bool InputManager::rebindKey(int playerIndex, int actionIndex, sf::Keyboard::Key newKey) {
    if (actionIndex < 0 || actionIndex >= ActionCount) return false;
    if (newKey == sf::Keyboard::Key::Unknown || isKeyAssigned(newKey)) return false;

    if (playerIndex == 0) {
        p1Keys[actionIndex] = newKey;
    } else {
        p2Keys[actionIndex] = newKey;
    }
    return true;
}

bool InputManager::isKeyAssigned(sf::Keyboard::Key key) const {
    if (key == sf::Keyboard::Key::Unknown) return false;
    for (int i = 0; i < ActionCount; ++i) {
        if (p1Keys[i] == key || p2Keys[i] == key) return true;
    }
    return false;
}

bool InputManager::isKeyAssignedExcept(sf::Keyboard::Key key, int playerIndex, int actionIndex) const {
    if (key == sf::Keyboard::Key::Unknown) return false;
    const auto* targetKeys = (playerIndex == 0) ? p1Keys : p2Keys;
    for (int i = 0; i < ActionCount; ++i) {
        if (i != actionIndex && targetKeys[i] == key) return true;
    }
    return false;
}

sf::Keyboard::Key InputManager::pollNewKey() const {
    static const sf::Keyboard::Key bindableKeys[] = {
        sf::Keyboard::Key::A, sf::Keyboard::Key::B, sf::Keyboard::Key::C, sf::Keyboard::Key::D,
        sf::Keyboard::Key::E, sf::Keyboard::Key::F, sf::Keyboard::Key::G, sf::Keyboard::Key::H,
        sf::Keyboard::Key::I, sf::Keyboard::Key::J, sf::Keyboard::Key::K, sf::Keyboard::Key::L,
        sf::Keyboard::Key::M, sf::Keyboard::Key::N, sf::Keyboard::Key::O, sf::Keyboard::Key::P,
        sf::Keyboard::Key::Q, sf::Keyboard::Key::R, sf::Keyboard::Key::S, sf::Keyboard::Key::T,
        sf::Keyboard::Key::U, sf::Keyboard::Key::V, sf::Keyboard::Key::W, sf::Keyboard::Key::X,
        sf::Keyboard::Key::Y, sf::Keyboard::Key::Z,
        sf::Keyboard::Key::Num0, sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2,
        sf::Keyboard::Key::Num3, sf::Keyboard::Key::Num4, sf::Keyboard::Key::Num5,
        sf::Keyboard::Key::Num6, sf::Keyboard::Key::Num7, sf::Keyboard::Key::Num8, sf::Keyboard::Key::Num9,
        sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Up, sf::Keyboard::Key::Down,
        sf::Keyboard::Key::Space, sf::Keyboard::Key::Enter, sf::Keyboard::Key::Tab,
        sf::Keyboard::Key::LControl, sf::Keyboard::Key::LShift, sf::Keyboard::Key::LAlt,
        sf::Keyboard::Key::RControl, sf::Keyboard::Key::RShift, sf::Keyboard::Key::RAlt,
        sf::Keyboard::Key::Numpad0, sf::Keyboard::Key::Numpad1, sf::Keyboard::Key::Numpad2,
        sf::Keyboard::Key::Numpad3, sf::Keyboard::Key::Numpad4, sf::Keyboard::Key::Numpad5,
        sf::Keyboard::Key::Numpad6, sf::Keyboard::Key::Numpad7, sf::Keyboard::Key::Numpad8,
        sf::Keyboard::Key::Numpad9
    };
    for (const auto key : bindableKeys) {
        if (pressed(key)) return key;
    }
    return sf::Keyboard::Key::Unknown;
}

const char* InputManager::getActionName(int actionIndex) {
    static const char* names[] = {
        "Move Left", "Move Right", "Jump", "Attack",
        "Dodge", "Use Item", "Prev Slot", "Next Slot",
        "Menu Up", "Menu Down", "Menu Left", "Menu Right",
        "Menu Confirm", "Menu Back", "Pause", "Info"
    };
    if (actionIndex >= 0 && actionIndex < ActionCount) return names[actionIndex];
    return "Unknown";
}

std::string InputManager::getKeyName(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Key::A: return "A";
        case sf::Keyboard::Key::B: return "B";
        case sf::Keyboard::Key::C: return "C";
        case sf::Keyboard::Key::D: return "D";
        case sf::Keyboard::Key::E: return "E";
        case sf::Keyboard::Key::F: return "F";
        case sf::Keyboard::Key::G: return "G";
        case sf::Keyboard::Key::H: return "H";
        case sf::Keyboard::Key::I: return "I";
        case sf::Keyboard::Key::J: return "J";
        case sf::Keyboard::Key::K: return "K";
        case sf::Keyboard::Key::L: return "L";
        case sf::Keyboard::Key::M: return "M";
        case sf::Keyboard::Key::N: return "N";
        case sf::Keyboard::Key::O: return "O";
        case sf::Keyboard::Key::P: return "P";
        case sf::Keyboard::Key::Q: return "Q";
        case sf::Keyboard::Key::R: return "R";
        case sf::Keyboard::Key::S: return "S";
        case sf::Keyboard::Key::T: return "T";
        case sf::Keyboard::Key::U: return "U";
        case sf::Keyboard::Key::V: return "V";
        case sf::Keyboard::Key::W: return "W";
        case sf::Keyboard::Key::X: return "X";
        case sf::Keyboard::Key::Y: return "Y";
        case sf::Keyboard::Key::Z: return "Z";
        case sf::Keyboard::Key::Num0: return "0";
        case sf::Keyboard::Key::Num1: return "1";
        case sf::Keyboard::Key::Num2: return "2";
        case sf::Keyboard::Key::Num3: return "3";
        case sf::Keyboard::Key::Num4: return "4";
        case sf::Keyboard::Key::Num5: return "5";
        case sf::Keyboard::Key::Num6: return "6";
        case sf::Keyboard::Key::Num7: return "7";
        case sf::Keyboard::Key::Num8: return "8";
        case sf::Keyboard::Key::Num9: return "9";
        case sf::Keyboard::Key::Escape: return "Esc";
        case sf::Keyboard::Key::LControl: return "LCtrl";
        case sf::Keyboard::Key::LShift: return "LShift";
        case sf::Keyboard::Key::LAlt: return "LAlt";
        case sf::Keyboard::Key::RControl: return "RCtrl";
        case sf::Keyboard::Key::RShift: return "RShift";
        case sf::Keyboard::Key::RAlt: return "RAlt";
        case sf::Keyboard::Key::Space: return "Space";
        case sf::Keyboard::Key::Enter: return "Enter";
        case sf::Keyboard::Key::Backspace: return "Back";
        case sf::Keyboard::Key::Tab: return "Tab";
        case sf::Keyboard::Key::PageUp: return "PgUp";
        case sf::Keyboard::Key::PageDown: return "PgDn";
        case sf::Keyboard::Key::End: return "End";
        case sf::Keyboard::Key::Home: return "Home";
        case sf::Keyboard::Key::Insert: return "Ins";
        case sf::Keyboard::Key::Delete: return "Del";
        case sf::Keyboard::Key::Left: return "Left";
        case sf::Keyboard::Key::Right: return "Right";
        case sf::Keyboard::Key::Up: return "Up";
        case sf::Keyboard::Key::Down: return "Down";
        case sf::Keyboard::Key::Numpad0: return "Pad 0";
        case sf::Keyboard::Key::Numpad1: return "Pad 1";
        case sf::Keyboard::Key::Numpad2: return "Pad 2";
        case sf::Keyboard::Key::Numpad3: return "Pad 3";
        case sf::Keyboard::Key::Numpad4: return "Pad 4";
        case sf::Keyboard::Key::Numpad5: return "Pad 5";
        case sf::Keyboard::Key::Numpad6: return "Pad 6";
        case sf::Keyboard::Key::Numpad7: return "Pad 7";
        case sf::Keyboard::Key::Numpad8: return "Pad 8";
        case sf::Keyboard::Key::Numpad9: return "Pad 9";
        default: return "Key#" + std::to_string(static_cast<int>(key));
    }
}

sf::Vector2i InputManager::getMousePosition() const {
    return mousePosition;
}

bool InputManager::mousePressed(sf::Mouse::Button button) const {
    return mouseDown(button) && std::find(previousMouseButtons.begin(), previousMouseButtons.end(), button) == previousMouseButtons.end();
}

bool InputManager::mouseDown(sf::Mouse::Button button) const {
    return std::find(currentMouseButtons.begin(), currentMouseButtons.end(), button) != currentMouseButtons.end();
}

float InputManager::getMouseWheelDelta() const {
    return mouseWheelDelta;
}

void InputManager::setMouseWheelDelta(float delta) {
    mouseWheelDelta = delta;
}

void InputManager::resetMouseWheelDelta() {
    mouseWheelDelta = 0.0f;
}
