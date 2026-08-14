#pragma once

#include "Entities/Entity.h"

class Projectile : public Entity {
public:
    Projectile(sf::Vector2f position, sf::Vector2f velocity, bool friendly);
    void update(float dt, const class TileMap& map);
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    bool isFriendly() const;

private:
    bool friendly = false;
    float life = 4.0f;
};

