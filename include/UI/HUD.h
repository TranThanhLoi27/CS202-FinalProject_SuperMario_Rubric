// Draws health, hunger, inventory, and respawn status for active players.
#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Level;
class Player;

class HUD {
public:
    /// Registers the shared UI font owned by AssetManager.
    static void setFont(const sf::Font& font);
    /// Registers shared HUD textures owned by AssetManager.
    static void setTextures(const sf::Texture& heart, const sf::Texture& meat,
                              const sf::Texture& food, const sf::Texture& coin, const sf::Texture& solid);
    /// Draws the complete HUD for every active player.
    void draw(sf::RenderWindow& window, const Level& level) const;
    /// Draws left-aligned text with the shared font.
    static void drawText(sf::RenderWindow& window, const std::string& text, sf::Vector2f position, unsigned size, sf::Color color);
    /// Draws text centered around a supplied position.
    static void drawTextCentered(sf::RenderWindow& window, const std::string& text, sf::Vector2f center, unsigned size, sf::Color color);
    /// Draws full, partial, and empty heart icons for a health value.
    static void drawHearts(sf::RenderWindow& window, sf::Vector2f position, float health, int maxHealth);
    /// Draws the player's hunger bar and icon.
    static void drawHungerBar(sf::RenderWindow& window, sf::Vector2f position, float hunger, float maxHunger);
    /// Draws a timed achievement notification.
    static void drawAchievement(sf::RenderWindow& window, const std::string& text, sf::Vector2f position, float timer);

private:
    /// Draws one player's name, resources, status, and inventory.
    static void drawPanel(sf::RenderWindow& window, const Player& player, sf::Vector2f position, const std::string& label);
    /// Draws a normalized filled status bar.
    static void drawBar(sf::RenderWindow& window, sf::Vector2f position, float width, float height, float ratio, sf::Color color);
};

