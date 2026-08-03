#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>
#include <World/SpikeTrap.h>
#include <string>
#include <vector>
struct LevelData{
    std::vector< std::vector<bool> > solidtiles;
    std::vector<sf::FloatRect> spikes;
    //std::vector<PlayerSpam> playerspams;
    //std::vector<GoalGate> goal;
    //std::vector<Enemy> enemies;
};

class LevelLoader{
public:
    bool load(const std::string& path, LevelData& out);
};