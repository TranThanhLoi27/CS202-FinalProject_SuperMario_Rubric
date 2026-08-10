#pragma once

#include "Entities/Entity.h"
#include <SFML/Graphics.hpp>

// Enum representing the type of item dropped in the game world
enum class ItemType {
    COIN,
    FOOD,
    HEART,
    BLOCK
};

// Represents a loot item floating in the world that players can pick up
class DroppedItem : public Entity {
private:
    ItemType type;
    sf::RectangleShape shape;
    
    float floatTimer; // Timer for floating sine wave animation
    sf::Vector2f basePosition; // Original position anchor

public:
    // Constructor creating a dropped item of specified type at given position
    DroppedItem(ItemType itemType, const sf::Vector2f& startPosition);
    virtual ~DroppedItem() = default;

    // Updates position with floating animation effect
    void update(float dt) override;
    
    // Renders item to window if active
    void draw(sf::RenderWindow& window) override;

    // Returns item type
    ItemType getType() const { return type; }
};
