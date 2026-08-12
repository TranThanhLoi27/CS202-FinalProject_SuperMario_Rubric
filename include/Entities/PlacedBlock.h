#pragma once

#include <SFML/Graphics.hpp>

// Represents a solid block placed by the player using inventory
class PlacedBlock {
private:
    sf::RectangleShape shape;
    sf::FloatRect bounds;

public:
    // Initializes block at specific position
    PlacedBlock(const sf::Vector2f& position);
    ~PlacedBlock() = default;

    // Renders block to window
    void draw(sf::RenderWindow& window) const;

    // Returns collision bounds
    sf::FloatRect getBounds() const { return bounds; }
};
