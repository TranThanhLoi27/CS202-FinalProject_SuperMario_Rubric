// Base entity with position, size, velocity, lifetime, and bounds.
#pragma once

#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity() = default;
    Entity(sf::Vector2f position, sf::Vector2f size);
    virtual ~Entity() = default;

    virtual void draw(sf::RenderWindow& window, sf::Vector2f camera) const = 0;
    sf::FloatRect getBounds() const;
    bool isAlive() const { return alive; }
    void kill() { alive = false; }

    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f velocity;
    bool onGround = false;
    int facingDirection = 1;

protected:
    bool alive = true;
};
