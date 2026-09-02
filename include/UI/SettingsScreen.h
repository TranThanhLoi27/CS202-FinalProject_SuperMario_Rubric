#pragma once

#include "Core/InputManager.h"

#include <SFML/Graphics.hpp>

#include <optional>
#include <string>

class SettingsScreen {
public:
    void updateHover(sf::Vector2f mousePosition);
    bool handleMouseClick(sf::Vector2f mousePosition);
    void handleKeyPressed(sf::Keyboard::Key key, InputManager& input);
    void draw(sf::RenderWindow& window, const InputManager& input) const;

    bool isWaitingForInput() const;
    std::size_t getSelectedPlayer() const;

private:
    static sf::FloatRect playerTabBounds(std::size_t playerIndex);
    static sf::FloatRect actionBounds(std::size_t actionIndex);
    static sf::FloatRect backButtonBounds();
    static void drawButton(sf::RenderWindow& window, const sf::FloatRect& bounds,
                           const std::string& label, bool hovered, bool selected = false);

    std::size_t selectedPlayer = 0;
    int hoveredPlayerTab = -1;
    int hoveredAction = -1;
    bool backHovered = false;
    std::optional<Action> waitingAction;
    std::string statusMessage;
};
