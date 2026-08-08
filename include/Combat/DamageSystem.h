#pragma once

class Level;
class Player;

class DamageSystem {
public:
    static void handlePlayerAttacks(Level& level);
    static void handleEnemyContact(Level& level);
    static void handleProjectiles(Level& level);
};
