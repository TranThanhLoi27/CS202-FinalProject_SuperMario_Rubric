#include "UI/HUD.h"
#include <algorithm>

// Initializes background and foreground rectangles for health/hunger UI
HUD::HUD() {
    float barMaxWidth = 200.0f;
    float barHeight = 15.0f;

    // --- Player 1 HUD Setup (Top-Left) ---
    p1HealthBg.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p1HealthBg.setFillColor(sf::Color(50, 50, 50));
    p1HealthBg.setPosition(sf::Vector2f(20.0f, 20.0f));

    p1HealthBar.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p1HealthBar.setFillColor(sf::Color::Red);
    p1HealthBar.setPosition(sf::Vector2f(20.0f, 20.0f));

    p1HungerBg.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p1HungerBg.setFillColor(sf::Color(50, 50, 50));
    p1HungerBg.setPosition(sf::Vector2f(20.0f, 45.0f));

    p1HungerBar.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p1HungerBar.setFillColor(sf::Color(255, 140, 0)); // Orange
    p1HungerBar.setPosition(sf::Vector2f(20.0f, 45.0f));

    // --- Player 2 HUD Setup (Top-Right) ---
    p2HealthBg.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p2HealthBg.setFillColor(sf::Color(50, 50, 50));
    p2HealthBg.setPosition(sf::Vector2f(560.0f, 20.0f));

    p2HealthBar.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p2HealthBar.setFillColor(sf::Color::Red);
    p2HealthBar.setPosition(sf::Vector2f(560.0f, 20.0f));

    p2HungerBg.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p2HungerBg.setFillColor(sf::Color(50, 50, 50));
    p2HungerBg.setPosition(sf::Vector2f(560.0f, 45.0f));

    p2HungerBar.setSize(sf::Vector2f(barMaxWidth, barHeight));
    p2HungerBar.setFillColor(sf::Color(255, 140, 0)); // Orange
    p2HungerBar.setPosition(sf::Vector2f(560.0f, 45.0f));
}

// Calculates percentage ratios and resizes the foreground bars accordingly
void HUD::update(float p1HP, float p1MaxHP, float p1Hunger, float p1MaxHunger,
                 float p2HP, float p2MaxHP, float p2Hunger, float p2MaxHunger) 
{
    float barMaxWidth = 200.0f;
    float barHeight = 15.0f;

    // Player 1 Ratios
    float p1HpRatio = (p1MaxHP > 0.0f) ? std::clamp(p1HP / p1MaxHP, 0.0f, 1.0f) : 0.0f;
    float p1HungerRatio = (p1MaxHunger > 0.0f) ? std::clamp(p1Hunger / p1MaxHunger, 0.0f, 1.0f) : 0.0f;

    p1HealthBar.setSize(sf::Vector2f(barMaxWidth * p1HpRatio, barHeight));
    p1HungerBar.setSize(sf::Vector2f(barMaxWidth * p1HungerRatio, barHeight));

    // Player 2 Ratios
    float p2HpRatio = (p2MaxHP > 0.0f) ? std::clamp(p2HP / p2MaxHP, 0.0f, 1.0f) : 0.0f;
    float p2HungerRatio = (p2MaxHunger > 0.0f) ? std::clamp(p2Hunger / p2MaxHunger, 0.0f, 1.0f) : 0.0f;

    p2HealthBar.setSize(sf::Vector2f(barMaxWidth * p2HpRatio, barHeight));
    p2HungerBar.setSize(sf::Vector2f(barMaxWidth * p2HungerRatio, barHeight));
}

// Renders the background and foreground bars to screen
void HUD::draw(sf::RenderWindow& window) {
    // Player 1
    window.draw(p1HealthBg);
    window.draw(p1HealthBar);
    window.draw(p1HungerBg);
    window.draw(p1HungerBar);

    // Player 2
    window.draw(p2HealthBg);
    window.draw(p2HealthBar);
    window.draw(p2HungerBg);
    window.draw(p2HungerBar);
}
