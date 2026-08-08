// // Resolves entity movement against solid tiles.
// #include "World/Collision.h"

// #include "Entities/Entity.h"
// #include "Utils/Constants.h"
// #include "Utils/MathUtils.h"
// #include "World/TileMap.h"

// #include <cmath>

// void Collision::resolveTileCollision(Entity& entity, const TileMap& map) {
//     entity.onGround = false;
//     entity.position.x += entity.velocity.x;

//     auto resolveAxis = [&](bool horizontal) {
//         sf::FloatRect bounds = entity.getBounds();
//         const int minTx = static_cast<int>(std::floor(bounds.position.x / Constants::TILE_SIZE));
//         const int maxTx = static_cast<int>(std::floor((bounds.position.x + bounds.size.x - 1.0f) / Constants::TILE_SIZE));
//         const int minTy = static_cast<int>(std::floor(bounds.position.y / Constants::TILE_SIZE));
//         const int maxTy = static_cast<int>(std::floor((bounds.position.y + bounds.size.y - 1.0f) / Constants::TILE_SIZE));

//         for (int ty = minTy; ty <= maxTy; ++ty) {
//             for (int tx = minTx; tx <= maxTx; ++tx) {
//                 if (!map.isSolidTile(tx, ty)) continue;
//                 const sf::FloatRect tile(
//                     {static_cast<float>(tx * Constants::TILE_SIZE), static_cast<float>(ty * Constants::TILE_SIZE)},
//                     {static_cast<float>(Constants::TILE_SIZE), static_cast<float>(Constants::TILE_SIZE)}
//                 );
//                 bounds = entity.getBounds();
//                 if (!MathUtils::intersects(bounds, tile)) continue;

//                 if (horizontal) {
//                     if (entity.velocity.x > 0.0f) entity.position.x = tile.position.x - entity.size.x;
//                     if (entity.velocity.x < 0.0f) entity.position.x = tile.position.x + tile.size.x;
//                     entity.velocity.x = 0.0f;
//                 } else {
//                     if (entity.velocity.y > 0.0f) {
//                         entity.position.y = tile.position.y - entity.size.y;
//                         entity.onGround = true;
//                     }
//                     if (entity.velocity.y < 0.0f) entity.position.y = tile.position.y + tile.size.y;
//                     entity.velocity.y = 0.0f;
//                 }
//             }
//         }
//     };

//     resolveAxis(true);
//     entity.position.y += entity.velocity.y;
//     resolveAxis(false);
// }
