// Tombstone stores a dead player's inventory until any player recovers it.
#pragma once

#include "Entities/Entity.h"
#include "Entities/Inventory.h"

class Tombstone : public Entity {
public:
    Tombstone(sf::Vector2f position, Inventory inventory, int ownerId);
    static void setTexture(const sf::Texture& texture);
    void update(float dt, const class TileMap& map);
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    const Inventory& getInventory() const;
    int getOwnerId() const;

private:
    Inventory inventory;
    int ownerId = 0;
};
