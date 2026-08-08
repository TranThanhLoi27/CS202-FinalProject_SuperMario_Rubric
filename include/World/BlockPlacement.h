// Validates and places runtime blocks next to a player.
#pragma once

#include <memory>
#include <vector>

class DroppedItem;
class Enemy;
class Player;
class TileMap;
class Tombstone;

class BlockPlacement {
public:
    static bool tryPlaceBlock(
        Player& player,
        TileMap& map,
        const std::vector<std::unique_ptr<Player>>& players,
        const std::vector<std::unique_ptr<Enemy>>& enemies,
        const std::vector<std::unique_ptr<DroppedItem>>& items,
        const std::vector<std::unique_ptr<Tombstone>>& tombstones
    );
};
