#pragma once

class Level;
class Player;

class DamageSystem {
public:
    /// Resolves active player attack boxes against every living enemy.
    static void handlePlayerAttacks(Level& level);
    /// Applies contact damage when a vulnerable player overlaps a living enemy.
    static void handleEnemyContact(Level& level);
    /// Resolves projectile collisions with players, enemies, and the world.
    static void handleProjectiles(Level& level);
};
