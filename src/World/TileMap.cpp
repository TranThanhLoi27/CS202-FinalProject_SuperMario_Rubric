#include <World/TileMap.h>

bool TileMap::IsSolid(const int& x, const int& y){
    if(x < 0 || y < 0 || x >= cols || y >= rows)return 0;
    return tiles[y][x];
}

void TileMap::Draw(sf::RenderWindow& window, sf::Vector2f camera) const {
    sf::RectangleShape tile({static_cast<float>(Constants::TILE_SIZE), static_cast<float>(Constants::TILE_SIZE)});
    //ve tam
    tile.setFillColor({57, 73, 50});
    sf::RectangleShape grass({static_cast<float>(Constants::TILE_SIZE), 6.0f});
    grass.setFillColor({86, 114, 74});
    const sf::Vector2f viewSize = window.getView().getSize();
    const int startX = std::max(0, static_cast<int>(camera.x / Constants::TILE_SIZE) - 1);
    const int endX = std::min(cols, static_cast<int>((camera.x + viewSize.x) / Constants::TILE_SIZE) + 2);
    const int startY = std::max(0, static_cast<int>(camera.y / Constants::TILE_SIZE) - 1);
    const int endY = std::min(rows, static_cast<int>((camera.y + viewSize.y) / Constants::TILE_SIZE) + 2);
    for(int y = startY; y <= endY; y++){
        for(int x = startX; x <= endX; x++){
            if(!tiles[y][x])continue;
            const sf::Vector2f p(static_cast<float>(x * Constants::TILE_SIZE) - camera.x, static_cast<float>(y * Constants::TILE_SIZE) - camera.y);
            tile.setPosition(p);
            window.draw(tile);
            grass.setPosition(p);
            window.draw(grass);
        }
    }
}