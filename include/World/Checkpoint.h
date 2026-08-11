// Marker data for checkpoint regions parsed from the tile map.
#pragma once

#include <SFML/Graphics/Rect.hpp>

class Checkpoint {
public:
    Checkpoint() = default;
    explicit Checkpoint(sf::FloatRect bounds);
    sf::FloatRect bounds;
    bool active = false;
};
