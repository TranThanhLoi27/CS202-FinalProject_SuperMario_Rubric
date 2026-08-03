#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class SpikeTrap{
public:
    void SetSpikes(std::vector<sf::FloatRect> _spikes);
private:
    std::vector<sf::FloatRect> spikes;
    int damage = 1;
};