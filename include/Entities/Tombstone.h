// Tombstone stores a dead player's inventory until any player recovers it.
#pragma once

#include "Entities/Entity.h"
#include "Entities/Inventory.h"

class Tombstone : public Entity {
public:
    /// Creates a tombstone containing one player's dropped inventory.
    Tombstone(sf::Vector2f position, Inventory inventory, int ownerId);
    /// Registers the shared tombstone texture.
    static void setTexture(const sf::Texture& texture);
    /// Applies gravity and resolves the tombstone against terrain.
    void update(float dt, const class TileMap& map);
    /// Draws the tombstone sprite or its fallback shape.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    /// Returns the inventory stored by the tombstone.
    const Inventory& getInventory() const;
    /// Returns the identifier of the player that created the tombstone.
    int getOwnerId() const;

private:
    Inventory inventory;
    int ownerId = 0;
};
