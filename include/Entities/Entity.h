// Base entity with position, size, velocity, lifetime, and bounds.
#pragma once

#include <SFML/Graphics.hpp>

class Entity {
public:
    /// Creates an entity with zeroed transform data.
    Entity() = default;
    /// Creates an entity with explicit world position and collision size.
    Entity(sf::Vector2f position, sf::Vector2f size);
    /// Enables safe destruction of derived entities through base pointers.
    virtual ~Entity() = default;

    /// Draws the entity relative to the active camera.
    virtual void draw(sf::RenderWindow& window, sf::Vector2f camera) const = 0;
    /// Returns the entity's current axis-aligned world bounds.
    sf::FloatRect getBounds() const;
    /// Reports whether the entity should remain in the level.
    bool isAlive() const { return alive; }
    /// Marks the entity for removal during the cleanup pass.
    void kill() { alive = false; }

    sf::Vector2f position;
    sf::Vector2f size;
    sf::Vector2f velocity;
    bool onGround = false;
    int facingDirection = 1;

protected:
    bool alive = true;
};

