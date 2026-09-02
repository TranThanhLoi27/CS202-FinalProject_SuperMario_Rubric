// Spritesheet animation metadata for future textured art.
#pragma once

#include <SFML/Graphics/Rect.hpp>

class Animation {
public:
    /// Creates a single-frame animation with default timing.
    Animation() = default;
    /// Creates animation metadata from a first frame, count, and frame duration.
    Animation(sf::IntRect firstFrame, int frameCount, float frameTime);
    /// Returns the texture rectangle for a zero-based animation frame.
    sf::IntRect frameRect(int frameIndex) const;
    sf::IntRect firstFrame;
    int frameCount = 1;
    float frameTime = 0.1f;
};
