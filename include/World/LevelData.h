#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>
#include <World/SpikeTrap.h>
#include <World/GoalGate.h>
#include <World/Checkpoint.h>

#include <string>
#include <vector>
struct LevelData{
    std::vector<std::vector<bool>> solidTiles;
    std::vector<SpikeTrap> spikes;
    std::vector<Checkpoint> checkpoints;
    std::vector<sf::Vector2f> playerSpawns;
    std::vector<std::pair<char, sf::Vector2f>> enemySpawns;
    std::vector<GoalGate> goals;
};
