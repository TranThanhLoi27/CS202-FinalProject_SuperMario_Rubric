#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>
#include <Utils/Constants.h>

#include <vector> 


class TileMap{
public:
    void setTiles(std::vector<std::vector<bool>> tiles);
    void Draw(sf::RenderWindow& window, sf::Vector2f camera, const sf::Texture* tileTexture = nullptr) const;
    void draw(sf::RenderWindow& window, sf::Vector2f camera, const sf::Texture* tileTexture = nullptr) const {
        Draw(window, camera, tileTexture);
    }
    bool IsSolid(const int& x, const int& y) const;
    bool isSolidTile(int x, int y) const { return IsSolid(x, y); }
    bool isSolidAt(sf::Vector2f position) const;
    bool setSolid(int x, int y, bool value = true);
    int widthPixels() const { return cols * Constants::TILE_SIZE; }
    int heightPixels() const { return rows * Constants::TILE_SIZE; }

private:
    int cols = 0, rows = 0;
    std::vector<std::vector<bool>> solid;
};
