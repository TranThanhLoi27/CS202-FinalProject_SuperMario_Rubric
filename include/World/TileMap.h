#pragma once

#include <SFML/Graphics.hpp>

#include <World/TileType.h>
#include <Utils/Constants.h>

#include <vector> 


class TileMap{
public:
    /// Replaces the solid-tile grid and updates cached dimensions.
    void setTiles(std::vector<std::vector<bool>> tiles);
    /// Draws visible solid tiles relative to the camera.
    void Draw(sf::RenderWindow& window, sf::Vector2f camera, const sf::Texture* tileTexture = nullptr) const;
    /// Provides the camel-case drawing wrapper used by newer callers.
    void draw(sf::RenderWindow& window, sf::Vector2f camera, const sf::Texture* tileTexture = nullptr) const {
        Draw(window, camera, tileTexture);
    }
    /// Reports whether an indexed tile is solid, treating out-of-range tiles as solid.
    bool IsSolid(const int& x, const int& y) const;
    /// Provides the camel-case solid-query wrapper used by newer callers.
    bool isSolidTile(int x, int y) const { return IsSolid(x, y); }
    /// Converts a world position to a tile and tests solidity.
    bool isSolidAt(sf::Vector2f position) const;
    /// Changes one in-range tile between air and solid.
    bool setSolid(int x, int y, bool value = true);
    /// Returns map width in world pixels.
    int widthPixels() const { return cols * Constants::TILE_SIZE; }
    /// Returns map height in world pixels.
    int heightPixels() const { return rows * Constants::TILE_SIZE; }

private:
    int cols = 0, rows = 0;
    std::vector<std::vector<bool>> solid;
};
