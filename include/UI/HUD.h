#pragma once

#include <SFML/Graphics.hpp>
#include "Utils/Constants.h"

// HUD class to display Health and Hunger bars for Player 1 and Player 2
class HUD {
private:
    // Background bars
    sf::RectangleShape p1HealthBg;
    sf::RectangleShape p1HungerBg;
    sf::RectangleShape p2HealthBg;
    sf::RectangleShape p2HungerBg;

    // Foreground dynamic bars
    sf::RectangleShape p1HealthBar;
    sf::RectangleShape p1HungerBar;
    sf::RectangleShape p2HealthBar;
    sf::RectangleShape p2HungerBar;

public:
    // Constructor setting up bar positions, dimensions, and colors
    HUD();
    ~HUD() = default;

    // Updates bar widths based on current stats percentage
    void update(float p1HP, float p1MaxHP, float p1Hunger, float p1MaxHunger,
                float p2HP, float p2MaxHP, float p2Hunger, float p2MaxHunger);

    // Draws all HUD elements onto the window
    void draw(sf::RenderWindow& window);
};
