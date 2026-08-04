#include<World/Level.h>


bool Level::LoadFromFile(const std::string& path){
    LevelData data;
    if(!LevelLoader().load(path, data))return false;
    map.setTiles(std::move(data.solidtiles));
    spikes = std::move(data.spikes);
    return true;
}

void Level::draw(sf::RenderWindow& window, sf::Vector2f& camera){
    map.Draw(window, camera);
    Draw(spikes, window, camera);
}