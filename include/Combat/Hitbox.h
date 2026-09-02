// Lightweight attack rectangle with a short active duration.
#pragma once

#include <SFML/Graphics/Rect.hpp>

class Hitbox {
public:
    /// Creates an inactive hitbox with default bounds and duration.
    Hitbox() = default;
    /// Creates a hitbox with the supplied world bounds and active duration.
    Hitbox(sf::FloatRect bounds, float duration);
    sf::FloatRect bounds;
    float duration = 0.0f;
};
