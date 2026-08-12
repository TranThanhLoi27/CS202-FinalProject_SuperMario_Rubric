#include "Entities/PlacedBlock.h"

// Initializes a 32x32 block with gray color and black outline
PlacedBlock::PlacedBlock(const sf::Vector2f& position) {
    shape.setSize(sf::Vector2f(32.0f, 32.0f));
    shape.setPosition(position);
    shape.setFillColor(sf::Color(169, 169, 169)); // Light Gray
    shape.setOutlineColor(sf::Color::Black);
    shape.setOutlineThickness(1.0f);
    bounds = sf::FloatRect(position, sf::Vector2f(32.0f, 32.0f));
}

// Draws the block to the given window
void PlacedBlock::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}
