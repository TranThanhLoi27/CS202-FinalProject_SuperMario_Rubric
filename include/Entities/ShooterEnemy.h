// Stationary enemy that aims at the closest living player and fires projectiles.
#pragma once

#include "Entities/Enemy.h"

class ShooterEnemy : public Enemy {
public:
    /// Creates a rooted mushroom shooter at its permanent anchor.
    explicit ShooterEnemy(sf::Vector2f position);
    /// Updates targeting, attacks, animation, and death without movement.
    void update(float dt, Level& level) override;
    /// Draws the mushroom's idle, attack, hurt, or death frame.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    /// Applies damage while rejecting all knockback and displacement.
    void takeDamage(int damage, Level& level, const Player& source) override;
    /// Returns zero while dying and contact damage otherwise.
    int getDamage() const override;

private:
    sf::Vector2f rootPosition;
    float shootCooldown = 1.2f;
    float attackTimer = 0.0f;
    float animationTime = 0.0f;
    float deathTimer = 0.0f;
    bool isDying = false;
};
