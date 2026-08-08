#include "Combat/DamageSystem.h"

#include "Entities/Enemy.h"
#include "Entities/Projectile.h"
#include "Utils/MathUtils.h"
#include "World/Level.h"

void DamageSystem::handlePlayerAttacks(Level& level) {
    for (auto& player : level.getPlayers()) {
        if (!player->isAttacking()) continue;
        const auto attack = player->attackBox();
        for (auto& enemy : level.getEnemies()) {
            if (MathUtils::intersects(attack, enemy->getBounds())) {
                enemy->takeDamage(1, level, *player);
            }
        }
    }
}

void DamageSystem::handleEnemyContact(Level& level) {
    for (auto& player : level.getPlayers()) {
        if (player->isRespawning()) continue;
        for (auto& enemy : level.getEnemies()) {
            if (!MathUtils::intersects(player->getBounds(), enemy->getBounds())) continue;
            const float knockback = player->position.x < enemy->position.x ? -230.0f : 230.0f;
            player->takeDamage(enemy->getDamage(), knockback);
        }
    }
}

void DamageSystem::handleProjectiles(Level& level) {
    for (auto& projectile : level.getProjectiles()) {
        if (!projectile->isAlive()) continue;
        if (projectile->isFriendly()) {
            for (auto& enemy : level.getEnemies()) {
                if (!MathUtils::intersects(projectile->getBounds(), enemy->getBounds())) continue;
                Player source(0, projectile->position, Player::profiles()[0]);
                source.facingDirection = projectile->velocity.x >= 0.0f ? 1 : -1;
                enemy->takeDamage(1, level, source);
                projectile->kill();
            }
        } else {
            for (auto& player : level.getPlayers()) {
                if (player->isRespawning() || player->isDodging() ||
                    !MathUtils::intersects(projectile->getBounds(), player->getBounds())) {
                    continue;
                }
                player->takeDamage(1, projectile->velocity.x >= 0.0f ? 180.0f : -180.0f);
                projectile->kill();
            }
        }
    }
}
