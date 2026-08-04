#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>
#include <World/SpikeTrap.h>
#include <string>
#include <vector>
struct LevelData{
    std::vector< std::vector<bool> > solidtiles;
    std::vector<SpikeTrap> spikes;
    //std::vector<PlayerSpam> playerspams;
    //std::vector<GoalGate> goal;
    //std::vector<Enemy> enemies;
};