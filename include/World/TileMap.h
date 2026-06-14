#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>

#include <vector> 


class TileMap{
public:
    void Draw();
    bool IsSolid(const int& x, const int& y);
private:
    int cols, rows;
    std::vector< std::vector<bool>> tiles;
};