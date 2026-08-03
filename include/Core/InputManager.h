#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

struct ActionState {
    bool isDown = false;     // State: Đang đè giữ phím (khi di chuyển)
    bool isPressed = false;  // State: Vừa nhấn xuống đúng 1 frame này (cho Chém, Đặt block, Quăng đồ)
};

// Every movement of a player
struct InputState {
    ActionState moveLeft;
    ActionState moveRight;
    ActionState jump;
    ActionState attack;
    ActionState dash;
    ActionState placeBlock;
    ActionState throwItem;
};

class InputManager{
private:
    // Save each player's state
    InputState p1State;
    InputState p2State;

    // Edge Detection
    struct PhysicalKeyboardState {
        // Player 1
        bool p1A = false, p1D = false, p1W = false;
        bool p1F = false, p1G = false, p1H = false, p1R = false;

        // Player 2
        bool p2Left = false, p2Right = false, p2Up = false;
        bool p2J = false, p2K = false, p2L = false, p2O = false;
    };

    PhysicalKeyboardState prevState;

    // Hàm phụ trợ tính toán trạng thái ActionState từ phím vật lý
    ActionState processKey(bool isCurrentlyDown, bool wasPreviouslyDown) {
        ActionState action;
        action.isDown = isCurrentlyDown;
        action.isPressed = isCurrentlyDown && !wasPreviouslyDown;
        return action;
    }

public:
    InputManager() = default;

    // Update
    void update();

    // Getters
    const InputState& getPlayer1Input() const;
    const InputState& getPlayer2Input() const;
};