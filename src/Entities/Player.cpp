#include "Entities/DroppedItem.h"
#include "Entities/Player.h"
#include "Entities/Tombstone.h"
#include "Utils/Constants.h"
#include "World/Collision.h"
#include "World/Level.h"

#include <algorithm>
#include <cstdint>

namespace {
float approach(float value, float target, float step) {
    return value < target ? std::min(value + step, target) : std::max(value - step, target);
}
}

const std::vector<Player::Profile>& Player::profiles() {
    static const std::vector<Profile> p = {
        {"ori", "High jump", "Jumps much higher.", "ori", {102, 183, 232}, 1, 1.22f, 1, 1, Constants::PLAYER_MAX_HEALTH, false},
        {"Rush", "Fast move", "Runs faster.", "rush", {105, 214, 146}, 1.26f, 1, 1, 1, Constants::PLAYER_MAX_HEALTH, false},
        {"Guard", "First hit guard", "Blocks first hit.", "rush", {238, 210, 96}, 1, 1, 1, 1, Constants::PLAYER_MAX_HEALTH, true},
        {"Titan", "Tank health", "More health.", "rush", {227, 164, 85}, .92f, .95f, 1, 1, Constants::PLAYER_MAX_HEALTH + 6, false},
        {"Feather", "Slow fall", "Falls slowly.", "rush", {194, 146, 232}, 1, 1, .72f, .66f, Constants::PLAYER_MAX_HEALTH, false},
        {"Legend", "All traits", "All traits.", "rush", {245, 111, 211}, 1.26f, 1.22f, .72f, .66f, Constants::PLAYER_MAX_HEALTH + 6, true}
    };
    return p;
}

Player::Player(int id, sf::Vector2f pos, Profile p)
    : Character(pos, {39.0f, 63.0f}, p.maxHealth),
      color(p.color),
      lastSafePosition(pos),
      id(id),
      spawn(pos),
      firstHitGuardAvailable(p.blocksFirstHit),
      sprites(CharacterSprites::get(p.spriteId)),
      profile(std::move(p)) {
    if (sprites) animator.play(sprites->idle);
}

void Player::updateAnimation(float dt) {
    if (!sprites) return;

    if (attackTimer > 0.0f) {
        animator.play(sprites->attack);
        animator.update(dt);
        return;
    }

    if (!onGround) {
        animator.play(sprites->jump);
        return;
    }

    if (std::abs(velocity.x) > sprites->runSpeedThreshold) {
        animator.play(sprites->run);
        animator.update(dt);
        return;
    }

    animator.play(sprites->idle);
}

void Player::update(float dt, const InputState& input, Level& level) {
    attackTimer = std::max(0.f, attackTimer - dt);
    dodgeTimer = std::max(0.f, dodgeTimer - dt);
    dodgeCooldownTimer = std::max(0.f, dodgeCooldownTimer - dt);
    hurtTimer = std::max(0.f, hurtTimer - dt);
    inventory.tick(dt);

    if (respawnTimer > 0) {
        if ((respawnTimer -= dt) <= 0) {
            position = spawn;
            velocity = {};
            health = maxHealth;
            hunger = 50;
            firstHitGuardAvailable = profile.blocksFirstHit;
            if (sprites) animator.play(sprites->idle);
        }
        return;
    }

    const int move = int(input.moveRight) - int(input.moveLeft);

    velocity.x = approach(
        velocity.x,
        move * Constants::PLAYER_SPEED * profile.speedMultiplier,
        (onGround
            ? Constants::PLAYER_GROUND_ACCELERATION
            : Constants::PLAYER_AIR_ACCELERATION) * dt
    );

    if (move != 0)
        facingDirection = move;

    coyoteTimer = onGround ? Constants::PLAYER_COYOTE_TIME : std::max(0.f, coyoteTimer - dt);

    if (input.jump && coyoteTimer > 0) {
        velocity.y =
            Constants::PLAYER_JUMP_SPEED * profile.jumpMultiplier;

        coyoteTimer = 0;
    }

    if (input.attack)
        attackTimer = 0.18f;

    if (input.dodge && dodgeCooldownTimer <= 0) {
        dodgeTimer = Constants::PLAYER_DODGE_DURATION;
        dodgeCooldownTimer = Constants::PLAYER_DODGE_COOLDOWN;
    }

    if (input.slotPrev) inventory.cycleSlot(-1);
    if (input.slotNext) inventory.cycleSlot(1);
    if (input.slotSelect >= 0) inventory.selectSlot(input.slotSelect);
    if (input.useItem) useSelectedItem(level);

    velocity.y = std::min(
        Constants::MAX_FALL_SPEED * profile.maxFallMultiplier,
        velocity.y +
            Constants::GRAVITY *
            profile.gravityMultiplier *
            dt
    );

    velocity *= dt;

    Collision::resolveTileCollision(
        *this,
        level.getTileMap()
    );

    velocity /= dt;

    updateAnimation(dt);

    if (onGround)
        lastSafePosition = position;

    if (position.y > level.getTileMap().heightPixels() + 260)
        die(level, true);
}

void Player::drawSprite(sf::RenderWindow& window, sf::Vector2f camera) const {
    if (!sprites || !sprites->texture) return;

    sf::Sprite sprite(*sprites->texture);
    const float scaleX = size.x / static_cast<float>(sprites->frameWidth);
    const float scaleY = size.y / static_cast<float>(sprites->frameHeight);

    sprite.setTextureRect(animator.getFrameRect());
    sprite.setOrigin({
        sprites->frameWidth * 0.5f,
        static_cast<float>(sprites->frameHeight)
    });
    sprite.setScale({
        facingDirection > 0 ? scaleX : -scaleX,
        scaleY
    });
    sprite.setPosition({
        position.x - camera.x + size.x * 0.5f,
        position.y - camera.y + size.y
    });

    // Apply dodge fading effect
    if (dodgeTimer > 0.0f) {
        const float fadeAlpha = (dodgeTimer / Constants::PLAYER_DODGE_DURATION) * 150.0f;
        const float finalAlpha = std::max(50.0f, fadeAlpha);
        sprite.setColor(sf::Color(255, 255, 255, static_cast<uint8_t>(finalAlpha)));
    }

    window.draw(sprite);
}

void Player::draw(
    sf::RenderWindow& window,
    sf::Vector2f camera
) const {
    if (isRespawning())
        return;

    if (sprites) {
        drawSprite(window, camera);
    } else {
        sf::RectangleShape body(size);

        body.setPosition(position - camera);

        body.setFillColor(
            sf::Color(
                color.r,
                color.g,
                color.b,
                static_cast<std::uint8_t>(
                    isDodging() ? 110 : 255
                )
            )
        );

        window.draw(body);
    }
}

void Player::takeDamage(int d, float knockback) { if (isRespawning()) return; if (firstHitGuardAvailable) { firstHitGuardAvailable = false; return; } Character::takeDamage(d, knockback); }
void Player::heal(int a) { health = std::min(maxHealth, health + a); }
void Player::restoreHunger(int a) { hunger = std::min<float>(Constants::PLAYER_MAX_HUNGER, hunger + a); }
void Player::die(Level& l, bool fell) { if (isRespawning()) return; auto stored = inventory.takeAll(); l.addTombstone(std::make_unique<Tombstone>(fell ? lastSafePosition : position, stored, id)); health = 0; respawnTimer = 2; diedBefore = true; recoveredTombstone = false; }
bool Player::isRespawning() const { return respawnTimer > 0; }
bool Player::isDodging() const { return dodgeTimer > 0 && !isRespawning(); }
bool Player::hasCollectedOwnTombstone() const { return recoveredTombstone; }
sf::FloatRect Player::attackBox() const { return {{facingDirection > 0 ? position.x + size.x - 3.0f : position.x - 51.0f, position.y + 12.0f}, {54.0f, 42.0f}}; }
bool Player::isAttacking() const { return attackTimer > 0 && !isRespawning(); }
int Player::getId() const { return id; }
int Player::getFacingDirection() const { return facingDirection; }
float Player::getHunger() const { return hunger; }
float Player::getRespawnTimer() const { return respawnTimer; }
const Player::Profile& Player::getProfile() const { return profile; }
sf::Vector2f Player::getSpawn() const { return spawn; }
void Player::setSpawn(sf::Vector2f p) { spawn = p; lastSafePosition = p; }
void Player::markTombstoneRecovered() { recoveredTombstone = true; }
bool Player::hasDiedBefore() const { return diedBefore; }
int Player::getSelectedSlot() const { return inventory.selectedSlot; }
float Player::getActionTimer() const { return inventory.actionTimer; }

void Player::useSelectedItem(Level& level) {
    if (!inventory.canAct() || isRespawning()) return;

    const int slot = inventory.selectedSlot;
    if (slot < FOOD_TYPE_COUNT) {
        if (inventory.getSlot(slot) <= 0) return;
        inventory.removeFromSlot(slot);
        restoreHunger(static_cast<int>(Constants::PLAYER_MAX_HUNGER * Constants::FOOD_HUNGER_PERCENT[slot]));
        heal(1);
        inventory.startAction(Constants::EAT_ACTION_TIME);
        return;
    }
    if (slot == COIN_SLOT_INDEX) {
        if (inventory.getSlot(slot) <= 0) return;
        inventory.removeFromSlot(slot);
        auto item = std::make_unique<DroppedItem>(
            position + sf::Vector2f(static_cast<float>(facingDirection) * 28.0f, 10.0f),
            ItemType::Coin,
            1
        );
        item->velocity = {static_cast<float>(facingDirection) * 330.0f, -240.0f};
        level.addDroppedItem(std::move(item));
        inventory.startAction(Constants::DROP_COIN_ACTION_TIME);
        return;
    }
    if (slot == BLOCK_SLOT_INDEX) {
        if (inventory.getSlot(slot) <= 0) return;
        if (level.tryPlaceBlock(*this)) {
            inventory.startAction(Constants::PLACE_BLOCK_ACTION_TIME);
        }
    }
}
