#pragma once

#include <SFML/System/Vector2.hpp>

#include <memory>

class Enemy;

/// Simple Factory that maps level symbols to concrete Enemy implementations.
class EnemyFactory {
public:
    /// Creates the enemy represented by a level symbol, or returns null for an unknown symbol.
    static std::unique_ptr<Enemy> create(char type, sf::Vector2f position);
};
