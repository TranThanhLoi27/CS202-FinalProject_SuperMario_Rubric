#pragma once

#include <SFML/Graphics.hpp>

#include <World/LevelData.h>
#include <World/TileType.h>
#include <World/SpikeTrap.h>
#include <string>
#include <vector>

class LevelLoader{
public:
    bool load(const std::string& path,
              LevelData& out,
              const sf::Texture& spikeTexture,
              const sf::Texture& goalTexture);
};
