#include "Entities/RangedEnemy.h"
#include <algorithm>

// Initializes brown color and shoot timer
RangedEnemy::RangedEnemy(const sf::Vector2f& startPosition) 
    : Enemy(startPosition), shootTimer(0.0f)
{
    body.setFillColor(sf::Color(139, 69, 19)); // Brown
    targetPos = sf::Vector2f(startPosition.x - 1.0f, startPosition.y); // Default aim direction left
}

// Spawns a new projectile every 2 seconds aimed at targetPos
void RangedEnemy::update(float dt) {
    if (!isAlive) return;

    shootTimer += dt;
    if (shootTimer >= 2.0f) {
        shootTimer = 0.0f;
        sf::Vector2f dir = targetPos - position;
        projectiles.emplace_back(position, dir);
    }

    // Update active projectiles
    for (auto& proj : projectiles) {
        proj.update(dt);
    }

    // Remove expired projectiles
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [](const Projectile& p) { return !p.getIsAlive(); }), projectiles.end());
}

// Renders enemy body and active projectiles
void RangedEnemy::draw(sf::RenderWindow& window) {
    if (!isAlive) return;
    
    window.draw(body);
    for (auto& proj : projectiles) {
        proj.draw(window);
    }
}
