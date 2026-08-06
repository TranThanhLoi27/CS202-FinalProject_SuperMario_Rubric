#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>
#include <World/SpikeTrap.h>
#include <World/GoalGate.h>

#include <string>
#include <vector>
struct LevelData{
    std::vector< std::vector<bool> > solidtiles;
    std::vector<SpikeTrap> spikes;
    std::vector<sf::Vector2f> playerspawns;
    std::vector<GoalGate> goals;
    //std::vector<Enemy> enemies;
};