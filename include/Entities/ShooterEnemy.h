// Stationary enemy that aims at the closest living player and fires projectiles.
#pragma once

#include "Entities/Enemy.h"

class ShooterEnemy : public Enemy {
public:
    explicit ShooterEnemy(sf::Vector2f position);
    void update(float dt, Level& level) override;
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    void takeDamage(int damage, Level& level, const Player& source) override;
    int getDamage() const override;

private:
    sf::Vector2f rootPosition;
    float shootCooldown = 1.2f;
    float attackTimer = 0.0f;
    float animationTime = 0.0f;
    float deathTimer = 0.0f;
    bool isDying = false;
};
