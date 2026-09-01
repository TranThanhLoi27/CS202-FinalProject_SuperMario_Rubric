#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <string>
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
    bool menuUp = false;
    bool menuDown = false;
    bool menuLeft = false;
    bool menuRight = false;
    bool menuConfirm = false;
    bool menuBack = false;
    bool pause = false;
    bool info = false;
};

class InputManager {
public:
    enum Action {
        MoveLeft = 0,
        MoveRight,
        Jump,
        Attack,
        Dodge,
        UseItem,
        SlotPrev,
        SlotNext,
        MenuUp,
        MenuDown,
        MenuLeft,
        MenuRight,
        MenuConfirm,
        MenuBack,
        Pause,
        Info,
        ActionCount
    };

    InputManager();
    void update();
    void update(const sf::WindowBase& window);
    const InputState& getPlayer1Input() const;
    const InputState& getPlayer2Input() const;
    bool pressed(sf::Keyboard::Key key) const;
    bool down(sf::Keyboard::Key key) const;

    sf::Keyboard::Key getKey(int playerIndex, int actionIndex) const;
    bool rebindKey(int playerIndex, int actionIndex, sf::Keyboard::Key newKey);
    bool isKeyAssigned(sf::Keyboard::Key key) const;
    bool isKeyAssignedExcept(sf::Keyboard::Key key, int playerIndex, int actionIndex) const;
    sf::Keyboard::Key pollNewKey() const;
    static std::string getKeyName(sf::Keyboard::Key key);
    static const char* getActionName(int actionIndex);

    // Mouse input methods
    sf::Vector2i getMousePosition() const;
    bool mousePressed(sf::Mouse::Button button) const;
    bool mouseDown(sf::Mouse::Button button) const;
    float getMouseWheelDelta() const;
    void setMouseWheelDelta(float delta);
    void resetMouseWheelDelta();

private:
    std::vector<sf::Keyboard::Key> relevantKeys;
    std::vector<sf::Keyboard::Key> currentKeys;
    std::vector<sf::Keyboard::Key> previousKeys;
    InputState player1;
    InputState player2;
    sf::Keyboard::Key p1Keys[16]; // Fixed size array for all actions (ActionCount = 16)
    sf::Keyboard::Key p2Keys[16]; // Fixed size array for all actions (ActionCount = 16)

    // Mouse input tracking
    sf::Vector2i mousePosition;
    std::vector<sf::Mouse::Button> currentMouseButtons;
    std::vector<sf::Mouse::Button> previousMouseButtons;
    float mouseWheelDelta;
};
