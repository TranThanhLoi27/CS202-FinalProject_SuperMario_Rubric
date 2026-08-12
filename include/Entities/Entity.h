#pragma once

#include <SFML/Graphics.hpp>

// Base abstract class for all game objects (Players, Enemies, Items, Projectiles)
class Entity {
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::FloatRect bounds;
    bool isAlive = true;

public:
    // Destructor
    virtual ~Entity() = default;

    // Updates entity logic per frame (movement, timers, animations)
    virtual void update(float dt) = 0;

    // Renders the entity onto the window
    virtual void draw(sf::RenderWindow& window) = 0;

    // Getters and Setters for position
    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f& newPosition) { position = newPosition; }

    // Getters and Setters for velocity
    sf::Vector2f getVelocity() const { return velocity; }
    void setVelocity(const sf::Vector2f& newVelocity) { velocity = newVelocity; }

    // Getters and Setters for collision bounds
    sf::FloatRect getBounds() const { return bounds; }
    void setBounds(const sf::FloatRect& newBounds) { bounds = newBounds; }

    // Check or change alive status
    bool getIsAlive() const { return isAlive; }
    void setIsAlive(bool alive) { isAlive = alive; }

    // Helper to destroy or remove the entity from game
    void kill() { isAlive = false; }
};