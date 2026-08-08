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

// void Level::tryPlaceBlock(Player& player) {
//     BlockPlacement::tryPlaceBlock(player, map, players, enemies, droppedItems, tombstones);
// }

// void Level::throwItem(Player& player) {
//     const ItemType order[] = {ItemType::Block, ItemType::Food, ItemType::Heart, ItemType::Coin};
//     for (ItemType type : order) {
//         if (player.getInventory().count(type) <= 0) continue;
//         player.getInventory().remove(type);
//         auto item = std::make_unique<DroppedItem>(
//             player.position + sf::Vector2f(static_cast<float>(player.getFacingDirection()) * 28.0f, 10.0f),
//             type,
//             1
//         );
//         item->velocity = {static_cast<float>(player.getFacingDirection()) * 330.0f, -240.0f};
//         addDroppedItem(std::move(item));
//         return;
//     }
// }

// TileMap& Level::getTileMap() { return map; }