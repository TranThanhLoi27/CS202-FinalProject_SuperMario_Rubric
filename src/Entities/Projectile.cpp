#include "Entities/Projectile.h"
#include <cmath>

// Normalizes direction vector and initializes speed/lifespan
Projectile::Projectile(const sf::Vector2f& startPos, const sf::Vector2f& dir) {
    position = startPos;
    
    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0) {
        direction = sf::Vector2f(dir.x / length, dir.y / length);
    } else {
        direction = sf::Vector2f(1.0f, 0.0f);
    }
    
    speed = 300.0f;
    lifespan = 5.0f;
    isAlive = true;

    shape.setSize(sf::Vector2f(8.0f, 8.0f));
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
    bounds = sf::FloatRect(sf::Vector2f(position.x, position.y), sf::Vector2f(8.0f, 8.0f));
}

// Moves bullet forward and destroys it after lifespan expires
void Projectile::update(float dt) {
    if (!isAlive) return;

    position += direction * speed * dt;
    bounds.position = position;
    shape.setPosition(position);

    lifespan -= dt;
    if (lifespan <= 0.0f) {
        kill();
    }
}

// Draws bullet shape to window
void Projectile::draw(sf::RenderWindow& window) {
    if (isAlive) {
        window.draw(shape);
    }
}
