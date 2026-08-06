#include<World/Level.h>


bool Level::LoadFromFile(const std::string& path, AssetManager& assets){
    LevelData data;
    if(!LevelLoader().load(path, data, assets))return false;
    map.setTiles(std::move(data.solidtiles));
    spikes = std::move(data.spikes);
    goals = std::move(data.goals);
    playerspawns = std::move(data.playerspawns);
    return true;
}

void Level::draw(sf::RenderWindow& window, sf::Vector2f& camera){
    map.Draw(window, camera);
    Draw(spikes, window, camera);
    Draw(goals, window, camera);
}