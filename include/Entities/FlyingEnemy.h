#pragma once

#include "Entities/Enemy.h"

class FlyingEnemy : public Enemy {
public:
    /// Creates a gravity-free flying enemy above its map spawn.
    explicit FlyingEnemy(sf::Vector2f position);
    /// Updates sinusoidal flight, tracking, knockback, collision, and death.
    void update(float dt, Level& level) override;
    /// Draws the appropriate flying, hurt, or death animation.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    /// Applies damage and a horizontal knockback impulse.
    void takeDamage(int damage, Level& level, const Player& source) override;
    /// Returns zero while dying and contact damage otherwise.
    int getDamage() const override;

private:
    float waveTime = 0.0f;
    sf::Vector2f origin;
    float animTime = 0.0f;
    float deathTimer = 0.0f;
    bool isDying = false;
};
