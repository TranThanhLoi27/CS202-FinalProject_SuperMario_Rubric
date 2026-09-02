#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/WindowBase.hpp>
#include <array>
#include <string>
#include <unordered_map>
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
    /// Command-style action identifiers decoupled from their physical keys.
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

    /// Associates logical commands with rebindable keyboard keys.
    using ActionBindings = std::unordered_map<Action, sf::Keyboard::Key>;

    /// Initializes the default action bindings for both players.
    InputManager();
    /// Polls keyboard and global mouse state and creates edge-triggered actions.
    void update();
    /// Polls input and records mouse coordinates relative to the game window.
    void update(const sf::WindowBase& window);
    /// Returns the current action snapshot for Player 1.
    const InputState& getPlayer1Input() const;
    /// Returns the current action snapshot for Player 2.
    const InputState& getPlayer2Input() const;
    /// Reports whether a key became pressed during the current frame.
    bool pressed(sf::Keyboard::Key key) const;
    /// Reports whether a key is currently held.
    bool down(sf::Keyboard::Key key) const;

    /// Returns the physical key assigned to a player's action.
    sf::Keyboard::Key getKey(int playerIndex, int actionIndex) const;
    /// Reassigns an action when the new key is valid and unused.
    bool rebindKey(int playerIndex, int actionIndex, sf::Keyboard::Key newKey);
    /// Reports whether either player already uses the requested key.
    bool isKeyAssigned(sf::Keyboard::Key key) const;
    /// Reports whether another action for the selected player uses the requested key.
    bool isKeyAssignedExcept(sf::Keyboard::Key key, int playerIndex, int actionIndex) const;
    /// Returns the first newly pressed key that can be assigned.
    sf::Keyboard::Key pollNewKey() const;
    /// Converts a keyboard key to a short UI label.
    static std::string getKeyName(sf::Keyboard::Key key);
    /// Converts an action index to its UI label.
    static const char* getActionName(int actionIndex);

    /// Returns the latest mouse position relative to the active window.
    sf::Vector2i getMousePosition() const;
    /// Reports whether a mouse button became pressed during the current frame.
    bool mousePressed(sf::Mouse::Button button) const;
    /// Reports whether a mouse button is currently held.
    bool mouseDown(sf::Mouse::Button button) const;
    /// Returns the accumulated mouse-wheel delta for the current frame.
    float getMouseWheelDelta() const;
    /// Stores an event-provided mouse-wheel delta.
    void setMouseWheelDelta(float delta);
    /// Clears the consumed mouse-wheel delta.
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
