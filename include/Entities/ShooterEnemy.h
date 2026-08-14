// Stationary enemy that aims at the closest living player and fires projectiles.
#pragma once

#include "Entities/Enemy.h"

class ShooterEnemy : public Enemy {
public:
    explicit ShooterEnemy(sf::Vector2f position);
    void update(float dt, Level& level) override;
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;

private:
    float shootCooldown = 1.2f;
};
