#include "Entities/FlyingEnemy.h"
#include <cmath>

// Initializes green color and animation parameters
FlyingEnemy::FlyingEnemy(const sf::Vector2f& startPosition) 
    : Enemy(startPosition), time(0.0f)
{
    speed = 2.0f;
    body.setFillColor(sf::Color::Green); // Green
}

// Moves enemy vertically using std::sin() wave pattern
void FlyingEnemy::update(float dt) {
    if (!isAlive) return;

    time += dt;
    position.y += std::sin(time) * speed;

    bounds.position = position;
    body.setPosition(position);
}
