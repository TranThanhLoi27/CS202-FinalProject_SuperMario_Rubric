// Draws health, hunger, inventory, and respawn status for active players.
#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Level;
class Player;

class HUD {
public:
    void draw(sf::RenderWindow& window, const Level& level) const;
    static void drawText(sf::RenderWindow& window, const std::string& text, sf::Vector2f position, unsigned size, sf::Color color);

private:
    static void drawPanel(sf::RenderWindow& window, const Player& player, sf::Vector2f position, const std::string& label);
    static void drawBar(sf::RenderWindow& window, sf::Vector2f position, float width, float height, float ratio, sf::Color color);
};
