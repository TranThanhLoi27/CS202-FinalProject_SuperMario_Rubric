#include<World/Level.h>

#include<World/LevelLoader.h>
bool Level::LoadFromFile(const std::string& path){
    LevelData data;
    if(!LevelLoader().load(path, data))return false;
    map.setTiles(std::move(data.solidtiles));
    spikes.SetSpikes(std::move(data.spikes));
    return true;
}

bool Level::draw(sf::RenderWindow& window, sf::Vector2f camera) const{
    map.Draw(window, camera);
    spikes.Draw(window, camera);
}