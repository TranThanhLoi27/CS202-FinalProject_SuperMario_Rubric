// Marker data for checkpoint regions parsed from the tile map.
#pragma once

#include <SFML/Graphics/Rect.hpp>

class Checkpoint {
public:
    /// Creates an inactive checkpoint with empty bounds.
    Checkpoint() = default;
    /// Creates an inactive checkpoint for a world-space trigger region.
    explicit Checkpoint(sf::FloatRect bounds);
    sf::FloatRect bounds;
    bool active = false;
    bool activated = false; // True when player has passed through it once
};
