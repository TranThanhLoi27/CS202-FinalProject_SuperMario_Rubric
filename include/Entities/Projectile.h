#pragma once
#include "Entities/Entity.h"
#include <SFML/Graphics.hpp>

// Projectile object shot by ranged enemies towards player
class Projectile : public Entity {
private:
    sf::Vector2f direction;
    float speed;
    sf::RectangleShape shape;
    float lifespan;

public:
    // Constructor specifying starting position and travel direction vector
    Projectile(const sf::Vector2f& startPos, const sf::Vector2f& dir);
    virtual ~Projectile() = default;

    // Updates projectile position along direction vector and checks lifespan
    void update(float dt) override;
    
    // Renders projectile shape if alive
    void draw(sf::RenderWindow& window) override;
};
