#pragma once

#include "Entities/Entity.h"

class Projectile : public Entity {
public:
    static void setTexture(const sf::Texture& texture);

    Projectile(sf::Vector2f position, sf::Vector2f velocity, bool friendly);
    void update(float dt, const class TileMap& map);
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    bool isFriendly() const;

private:
    bool friendly = false;
    float life = 4.0f;
    float animTime = 0.0f;
};

