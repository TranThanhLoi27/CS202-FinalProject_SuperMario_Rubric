// Larger enemy with high health and repeated projectile attacks.
#pragma once

#include "Entities/Enemy.h"

class BossEnemy : public Enemy {
public:
    explicit BossEnemy(sf::Vector2f position);
    void update(float dt, Level& level) override;
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    void takeDamage(int damage, Level& level, const Player& source) override;
    int getDamage() const override;

private:
    float shootCooldown = 1.0f;
    float animTime = 0.0f;
    float deathTimer = 0.0f;
    bool isDying = false;
    bool isAttacking = false;
    bool hasFired = false;
};
