#pragma once
#include "Entities/Enemy.h"
#include "Entities/Projectile.h"
#include <vector>

// Stationary enemy that shoots projectiles towards target position every 2 seconds
class RangedEnemy : public Enemy {
private:
    float shootTimer;
    std::vector<Projectile> projectiles;
    sf::Vector2f targetPos;

public:
    /// Creates the legacy ranged-enemy prototype at a starting position.
    RangedEnemy(const sf::Vector2f& startPosition);
    /// Enables safe destruction through the ranged-enemy type.
    virtual ~RangedEnemy() = default;

    /// Updates the legacy shoot timer and its locally owned projectiles.
    void update(float dt) override;
    
    /// Draws the legacy enemy and its locally owned projectiles.
    void draw(sf::RenderWindow& window) override;

    /// Updates the world position used for aiming.
    void setTargetPosition(const sf::Vector2f& target) { targetPos = target; }
    
    /// Returns mutable access to legacy locally owned projectiles.
    std::vector<Projectile>& getProjectiles() { return projectiles; }
};
