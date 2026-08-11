// Lightweight attack rectangle with a short active duration.
#pragma once

#include <SFML/Graphics/Rect.hpp>

class Hitbox {
public:
    Hitbox() = default;
    Hitbox(sf::FloatRect bounds, float duration);
    sf::FloatRect bounds;
    float duration = 0.0f;
};
