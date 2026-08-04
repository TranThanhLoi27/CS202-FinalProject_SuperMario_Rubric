#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class SpikeTrap{
public:
    SpikeTrap(int damage = 1, sf::FloatRect bound, sf::Sprite);
private:
    sf::Sprite sprite;
    sf::FloatRect bound;
    int damage = 1;
};