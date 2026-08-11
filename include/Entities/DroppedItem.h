// Physics item on the ground that players can collect or throw to each other.
#pragma once

#include "Entities/Entity.h"
#include "Entities/Inventory.h"

class DroppedItem : public Entity {
public:
    DroppedItem(sf::Vector2f position, ItemType type, int quantity = 1);
    void update(float dt, const class TileMap& map);
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;

    ItemType getType() const;
    int getQuantity() const;

private:
    ItemType type;
    int quantity = 1;
    float age = 0.0f;
};
