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
    bool LoadFromFile(const std::string& path, AssetManager& assets);
    void draw(sf::RenderWindow& window, sf::Vector2f& camera);

private:
    TileMap map;
    std::vector<SpikeTrap> spikes;
    std::vector<GoalGate> goals;
    std::vector<sf::Vector2f> playerspawns;
};

template <typename T>
bool Isbound(T obj, sf::RenderWindow& window, sf::Vector2f& camera){
    sf::Vector2f ViewSize = window.getView().getSize();
    sf::FloatRect bound = obj.getBound();
    if(bound.position.x < camera.x  / Constants::TILE_SIZE || bound.position.x >= (camera.x + ViewSize.x) / Constants::TILE_SIZE )return 0;
    if(bound.position.y < camera.y  / Constants::TILE_SIZE || bound.position.y >= (camera.y + ViewSize.y) / Constants::TILE_SIZE )return 0;
    return 1;   
}

template <typename T>
void Draw(std::vector<T>& Vec, sf::RenderWindow& window, sf::Vector2f& camera){
    for(const T& e: Vec){
        if(Isbound(e, window, camera))e.render(window);
    }
}