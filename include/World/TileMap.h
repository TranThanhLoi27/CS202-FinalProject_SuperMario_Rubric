#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>
#include <Utils/Constants.h>

#include <vector> 


class TileMap{
public:
    void setTiles(std::vector<std::vector<bool>> tiles);
    void Draw(sf::RenderWindow& window, sf::Vector2f camera) const;
    bool IsSolid(const int& x, const int& y);
private:
    int cols, rows;
    std::vector< std::vector<bool>> solid;
};