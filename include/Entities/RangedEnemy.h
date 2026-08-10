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
    // Constructor setting start position
    RangedEnemy(const sf::Vector2f& startPosition);
    virtual ~RangedEnemy() = default;

    // Updates shoot timer, spawns projectiles, and updates active projectiles
    void update(float dt) override;
    
    // Draws enemy shape and active projectiles
    void draw(sf::RenderWindow& window) override;

    // Updates target position (e.g. Player position) for aiming
    void setTargetPosition(const sf::Vector2f& target) { targetPos = target; }
    
    // Returns active projectiles vector
    std::vector<Projectile>& getProjectiles() { return projectiles; }
};
