#pragma once

#include <SFML/Graphics.hpp>

// Represents a solid block placed by the player using inventory
class PlacedBlock {
private:
    sf::RectangleShape shape;
    sf::FloatRect bounds;

public:
    /// Initializes a block at the requested world position.
    PlacedBlock(const sf::Vector2f& position);
    /// Destroys the block using normal value semantics.
    ~PlacedBlock() = default;

    /// Draws the block using its rectangle shape.
    void draw(sf::RenderWindow& window) const;

    /// Returns the collision bounds of the placed block.
    sf::FloatRect getBounds() const { return bounds; }
};
