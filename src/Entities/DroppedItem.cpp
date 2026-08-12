#include "Entities/DroppedItem.h"
#include <cmath>

// Initializes shape size, position, and color based on ItemType
DroppedItem::DroppedItem(ItemType itemType, const sf::Vector2f& startPosition) 
    : type(itemType), floatTimer(0.0f)
{
    position = startPosition;
    basePosition = startPosition;
    velocity = sf::Vector2f(0.0f, 0.0f);
    bounds = sf::FloatRect(sf::Vector2f(position.x, position.y), sf::Vector2f(16.0f, 16.0f));
    isAlive = true;

    shape.setSize(sf::Vector2f(16.0f, 16.0f));
    shape.setPosition(position);

    // Set colors for distinct item recognition: Yellow = Coin, Orange = Food, Red = Heart, Gray = Block
    switch (type) {
        case ItemType::COIN:
            shape.setFillColor(sf::Color::Yellow);
            break;
        case ItemType::FOOD:
            shape.setFillColor(sf::Color(255, 165, 0)); // Orange
            break;
        case ItemType::HEART:
            shape.setFillColor(sf::Color::Red);
            break;
        case ItemType::BLOCK:
            shape.setFillColor(sf::Color(128, 128, 128)); // Gray
            break;
    }
}

// Moves item up and down smoothly using std::sin() for a floating effect
void DroppedItem::update(float dt) {
    floatTimer += dt;
    
    float offsetY = std::sin(floatTimer * 4.0f) * 5.0f;
    position.y = basePosition.y + offsetY;
    
    bounds.position = position;
    shape.setPosition(position);
}

// Draws the item if alive
void DroppedItem::draw(sf::RenderWindow& window) {
    if (getIsAlive()) {
        window.draw(shape);
    }
}
