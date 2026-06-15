#include <World/TileMap.h>

bool TileMap::IsSolid(const int& x, const int& y){
    if(x < 0 || y < 0 || x >= cols || y >= rows)return 0;
    return tiles[y][x];
}

// void TileMap::Draw(contst sf::VideoMode& window){
    
// }