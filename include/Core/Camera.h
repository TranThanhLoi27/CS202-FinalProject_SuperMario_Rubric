#pragma once

#include <SFML/Graphics.hpp>

class Camera{
public:
    /// Smoothly follows a target while clamping the view to the world boundaries.
    static sf::Vector2f follow(sf::Vector2f current, sf::Vector2f target, sf::Vector2f viewSize, sf::Vector2f worldSize, float dt);
    /// Resets any future camera-owned state to its default value.
    void reset();
};
