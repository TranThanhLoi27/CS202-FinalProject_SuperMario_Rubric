// Ground enemy that walks back and forth and turns at walls or ledges.
#pragma once

#include "Entities/Enemy.h"

class PatrolEnemy : public Enemy {
public:
    /// Creates a ground patrol enemy at its map spawn.
    explicit PatrolEnemy(sf::Vector2f position);
    /// Updates patrol movement, gravity, collision turnaround, and death.
    void update(float dt, Level& level) override;
    /// Draws the patrol sprite, hurt feedback, and health bar.
    void draw(sf::RenderWindow& window, sf::Vector2f camera) const override;
    /// Applies damage and source-directed horizontal knockback.
    void takeDamage(int damage, Level& level, const Player& source) override;
    /// Returns zero while dying and contact damage otherwise.
    int getDamage() const override;

private:
    float animTime = 0.0f;
    float deathTimer = 0.0f;
    bool isDying = false;
};
