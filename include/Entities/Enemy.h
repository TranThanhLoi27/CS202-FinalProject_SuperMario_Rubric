// Base enemy behavior for health, knockback, loot drop, and health bar drawing.
#pragma once

#include "Entities/Character.h"

class Level;
class Player;

// Base class for all enemy types inheriting from Character
class Enemy : public Character {
public:
    Enemy(sf::Vector2f position, sf::Vector2f size, int health, sf::Color color);
    virtual ~Enemy() = default;
    virtual void update(float dt, Level& level) = 0;
    using Character::takeDamage;
    virtual void takeDamage(int damage, Level& level, const Player& source);
    void addMaxHealth(int amount);
    virtual int getDamage() const;

protected:
    void tick(float dt);
    void drawBody(sf::RenderWindow& window, sf::Vector2f camera) const;
    void drawSprite(sf::RenderWindow& window, sf::Vector2f camera, const sf::Texture& texture) const;
    void drawHealthBar(sf::RenderWindow& window, sf::Vector2f camera) const;
    sf::Color bodyColor;
    int damage = 1;
    float hitTimer = 0.0f;
};

