#pragma once
#include "Entities/Character.h"
#include <SFML/Graphics.hpp>

// Base class for all enemy types inheriting from Character
class Enemy : public Character {
protected:
    float damage = 10.0f;
    int scoreValue = 100;
    sf::RectangleShape body;

public:
    // Constructor placing enemy at starting position
    Enemy(const sf::Vector2f& startPosition);
    virtual ~Enemy() = default;

    // Reduces enemy health and marks isAlive as false if health reaching 0
    virtual void takeDamage(float amount);

    // Pure virtual update method to be implemented by enemy subtypes
    virtual void update(float dt) = 0;

    // Renders the enemy shape to screen
    virtual void draw(sf::RenderWindow& window) override;

    // Getters for enemy attributes
    float getDamage() const { return damage; }
    int getScoreValue() const { return scoreValue; }
};
