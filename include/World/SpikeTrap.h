#pragma once

#include <SFML/Graphics.hpp>

class SpikeTrap{
public:
    SpikeTrap(sf::FloatRect bound, int damage = 1);
private:
    sf::FloatRect bound;
    int damage = 1;
};