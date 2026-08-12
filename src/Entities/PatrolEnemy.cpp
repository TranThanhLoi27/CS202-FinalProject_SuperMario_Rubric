#include "Entities/PatrolEnemy.h"

// Initializes purple color and patrol parameters
PatrolEnemy::PatrolEnemy(const sf::Vector2f& startPosition, float distance) 
    : Enemy(startPosition), patrolDistance(distance)
{
    startX = startPosition.x;
    speed = 50.0f;
    direction = 1;
    
    body.setFillColor(sf::Color(128, 0, 128)); // Purple
}

// Moves enemy left and right between startX - distance and startX + distance
void PatrolEnemy::update(float dt) {
    if (!isAlive) return;

    position.x += speed * direction * dt;

    if (position.x > startX + patrolDistance) {
        position.x = startX + patrolDistance;
        direction = -1;
    } else if (position.x < startX - patrolDistance) {
        position.x = startX - patrolDistance;
        direction = 1;
    }

    bounds.position = position;
    body.setPosition(position);
}
