#pragma once

#include<SFML/Graphics.hpp>

#include<World/TileMap.h>
#include<World/SpikeTrap.h>

#include<vector>
class Level{
public:
    bool LoadFromFile(const std::string& path);
    bool draw(sf::RenderWindow& window, sf::Vector2f camera) const;

private:
    TileMap map;
    SpikeTrap spikes;
};