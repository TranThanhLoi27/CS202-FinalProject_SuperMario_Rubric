// Spritesheet animation metadata for future textured art.
#pragma once

#include <SFML/Graphics/Rect.hpp>

class Animation {
public:
    Animation() = default;
    Animation(sf::IntRect firstFrame, int frameCount, float frameTime);
    sf::IntRect firstFrame;
    int frameCount = 1;
    float frameTime = 0.1f;
};
