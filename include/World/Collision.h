#pragma once

class Entity;
class TileMap;

class Collision {
public:
    static void resolveTileCollision(Entity& entity, const TileMap& map);
};
