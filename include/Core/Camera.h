#pragma once

#include <SFML/Graphics.hpp>

class Camera{
public:
    static sf::Vector2f follow(sf::Vector2f current, sf::Vector2f target, sf::Vector2f viewSize, sf::Vector2f worldSize, float dt);
    void reset();
};