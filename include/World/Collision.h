#pragma once

class Entity;
class TileMap;

class Collision {
public:
    /// Resolves axis-separated entity movement against solid map tiles.
    static void resolveTileCollision(Entity& entity, const TileMap& map);
};
