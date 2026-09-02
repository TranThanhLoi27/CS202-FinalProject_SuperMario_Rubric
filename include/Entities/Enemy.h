// Base enemy behavior for health, knockback, loot drop, and health bar drawing.
#pragma once

#include "Entities/Character.h"

class Level;
class Player;

// Base class for all enemy types inheriting from Character
class Enemy : public Character {
public:
    /// Creates the shared health, transform, and fallback visual state for an enemy.
    Enemy(sf::Vector2f position, sf::Vector2f size, int health, sf::Color color);
    /// Enables safe polymorphic destruction through Enemy pointers.
    virtual ~Enemy() = default;
    /// Updates concrete enemy behavior against the current level.
    virtual void update(float dt, Level& level) = 0;
    using Character::takeDamage;
    /// Applies source-aware damage, knockback, and death/loot behavior.
    virtual void takeDamage(int damage, Level& level, const Player& source);
    /// Raises current and maximum health for difficulty scaling.
    void addMaxHealth(int amount);
    /// Returns contact damage dealt to players.
    virtual int getDamage() const;

protected:
    /// Advances shared enemy hurt-state timers.
    void tick(float dt);
    /// Draws the fallback colored enemy body.
    void drawBody(sf::RenderWindow& window, sf::Vector2f camera) const;
    /// Draws a centered and direction-aware enemy texture.
    void drawSprite(sf::RenderWindow& window, sf::Vector2f camera, const sf::Texture& texture) const;
    /// Draws the enemy health bar above its body.
    void drawHealthBar(sf::RenderWindow& window, sf::Vector2f camera) const;
    sf::Color bodyColor;
    int damage = 1;
    float hitTimer = 0.0f;
};

