#pragma once

#include<SFML/Graphics.hpp>

#include<World/LevelLoader.h>
#include<World/TileMap.h>
#include<World/SpikeTrap.h>
#include<Utils/Constants.h>

#include<string>
#include<vector>
class Level{
public:
    bool LoadFromFile(const std::string& path);
    void draw(sf::RenderWindow& window, sf::Vector2f& camera);

private:
    TileMap map;
    std::vector<SpikeTrap> spikes;
};

template <typename T>
bool Isbound(T obj, sf::RenderWindow& window, sf::Vector2f& camera){
    sf::Vector2f ViewSize = window.getView().getSize();
    if(obj.bound.position.x < camera.x  / Constants::Tile_Size || obj.bound.position.x >= (camera.x + ViewSize.x) / Constants::Tile_Size )return 0;
    if(obj.bound.position.y < camera.y  / Constants::Tile_Size || obj.bound.position.y >= (camera.y + ViewSize.y) / Constants::Tile_Size )return 0;
    return 1;   
}

template <typename T>
void Draw(std::vector<T>& Vec, sf::RenderWindow& window, sf::Vector2f& camera){
    for(T e: Vec){
        if(Isbound(e, window, camera))window.draw(e.sprite);
    }
}