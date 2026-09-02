// Validates block placement against terrain and all gameplay objects.
#include "World/BlockPlacement.h"

#include "Entities/DroppedItem.h"
#include "Entities/Enemy.h"
#include "Entities/Inventory.h"
#include "Entities/Player.h"
#include "Entities/Tombstone.h"
#include "Utils/Constants.h"
#include "Utils/MathUtils.h"
#include "World/TileMap.h"

#include <cmath>

namespace {
int tileAt(float worldCoordinate) {
    return static_cast<int>(std::floor(worldCoordinate / Constants::TILE_SIZE));
}

sf::FloatRect tileRect(int tx, int ty) {
    return {
        {static_cast<float>(tx * Constants::TILE_SIZE), static_cast<float>(ty * Constants::TILE_SIZE)},
        {static_cast<float>(Constants::TILE_SIZE), static_cast<float>(Constants::TILE_SIZE)}
    };
}

bool overlapsAnyObject(
    const sf::FloatRect& blockRect,
    const std::vector<std::unique_ptr<Player>>& players,
    const std::vector<std::unique_ptr<Enemy>>& enemies,
    const std::vector<std::unique_ptr<DroppedItem>>& items,
    const std::vector<std::unique_ptr<Tombstone>>& tombstones
) {
    for (const auto& p : players) if (!p->isRespawning() && MathUtils::intersects(blockRect, p->getBounds())) return true;
    for (const auto& e : enemies) if (MathUtils::intersects(blockRect, e->getBounds())) return true;
    for (const auto& i : items) if (MathUtils::intersects(blockRect, i->getBounds())) return true;
    for (const auto& t : tombstones) if (MathUtils::intersects(blockRect, t->getBounds())) return true;
    return false;
}
}

bool BlockPlacement::tryPlaceBlock(
    Player& player,
    TileMap& map,
    const std::vector<std::unique_ptr<Player>>& players,
    const std::vector<std::unique_ptr<Enemy>>& enemies,
    const std::vector<std::unique_ptr<DroppedItem>>& items,
    const std::vector<std::unique_ptr<Tombstone>>& tombstones
) {
    if (player.getInventory().getSlot(BLOCK_SLOT_INDEX) <= 0 || player.isRespawning()) return false;

    const auto b = player.getBounds();
    const int direction = player.getFacingDirection() >= 0 ? 1 : -1;
    const int ty = tileAt(b.position.y + b.size.y * 0.5f);
    const int startTx = direction > 0
        ? tileAt(b.position.x + b.size.x)
        : tileAt(b.position.x - 1.0f);
    const int endTx = direction > 0
        ? map.widthPixels() / Constants::TILE_SIZE
        : -1;

    for (int tx = startTx; tx != endTx; tx += direction) {
        if (map.isSolidTile(tx, ty)) continue;

        const sf::FloatRect blockRect = tileRect(tx, ty);
        if (MathUtils::intersects(blockRect, b)) continue;
        if (overlapsAnyObject(blockRect, players, enemies, items, tombstones)) continue;

        if (!map.setSolid(tx, ty)) continue;
        player.recordPlacedBlock(tx, ty);
        player.getInventory().removeFromSlot(BLOCK_SLOT_INDEX);
        return true;
    }

    return false;
}
