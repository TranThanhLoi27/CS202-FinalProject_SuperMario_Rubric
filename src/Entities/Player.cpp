// // Implements player input movement, attack timing, hunger, death, and drawing.
// #include "Entities/Player.h"

// //#include "Entities/Tombstone.h"
// #include "Utils/Constants.h"
// #include "World/Collision.h"
// #include "World/Level.h"

// #include <algorithm>
// #include <cstdint>
// #include <utility>

// namespace {
// float approach(float current, float target, float maxDelta) {
//     if (current < target) return std::min(current + maxDelta, target);
//     if (current > target) return std::max(current - maxDelta, target);
//     return target;
// }
// }

// const std::vector<Player::Profile>& Player::profiles() {
//     static const std::vector<Profile> available = {
//         {"Sky", "High jump", "Jumps much higher than others.", {102, 183, 232}, 1.0f, 1.22f, 1.0f, 1.0f, Constants::PLAYER_MAX_HEALTH, false},
//         {"Rush", "Fast move", "Runs faster on flat ground.", {105, 214, 146}, 1.26f, 1.0f, 1.0f, 1.0f, Constants::PLAYER_MAX_HEALTH, false},
//         {"Guard", "First hit guard", "Ignores the first hit after spawning.", {238, 210, 96}, 1.0f, 1.0f, 1.0f, 1.0f, Constants::PLAYER_MAX_HEALTH, true},
//         {"Titan", "Tank health", "Starts with more maximum health.", {227, 164, 85}, 0.92f, 0.95f, 1.0f, 1.0f, Constants::PLAYER_MAX_HEALTH + 3, false},
//         {"Feather", "Slow fall", "Falls slower and is easier to control in air.", {194, 146, 232}, 1.0f, 1.0f, 0.72f, 0.66f, Constants::PLAYER_MAX_HEALTH, false},
//         {"Legend", "All traits", "High jump, fast run, guard, tank health, and slow fall.", {245, 111, 211}, 1.26f, 1.22f, 0.72f, 0.66f, Constants::PLAYER_MAX_HEALTH + 3, true}
//     };
//     return available;
// }

// Player::Player(int id, sf::Vector2f spawnPosition, Profile profile)
//     : Character(spawnPosition, {26.0f, 42.0f}, profile.maxHealth),
//       color(profile.color),
//       lastSafePosition(spawnPosition),
//       id(id),
//       spawn(spawnPosition),
//       firstHitGuardAvailable(profile.blocksFirstHit),
//       profile(std::move(profile)) {
//    // inventory.block = Constants::START_BLOCK_COUNT;
// }

// void Player::update(float dt, const InputState& input, Level& level) {
//     attackTimer = std::max(0.0f, attackTimer - dt);
//     dodgeTimer = std::max(0.0f, dodgeTimer - dt);
//     dodgeCooldownTimer = std::max(0.0f, dodgeCooldownTimer - dt);
//     hurtTimer = std::max(0.0f, hurtTimer - dt);

//     if (respawnTimer > 0.0f) {
//         respawnTimer -= dt;
//         if (respawnTimer <= 0.0f) {
//             position = spawn;
//             velocity = {};
//             health = maxHealth;
//             hunger = 50.0f;
//             dodgeTimer = 0.0f;
//             dodgeCooldownTimer = 0.0f;
//             coyoteTimer = 0.0f;
//             jumpBufferTimer = 0.0f;
//             firstHitGuardAvailable = profile.blocksFirstHit;
//         }
//         return;
//     }

//     hunger = std::max(0.0f, hunger - Constants::HUNGER_LOSS_PER_SECOND * dt);
//     if (hunger <= 0.0f) {
//         hungerDamageTimer += dt;
//         if (hungerDamageTimer >= Constants::HUNGER_DAMAGE_INTERVAL) {
//             hungerDamageTimer = 0.0f;
//             takeDamage(1);
//         }
//     }

//     int move = 0;
//     if (input.moveLeft) --move;
//     if (input.moveRight) ++move;
//     const float targetSpeed = static_cast<float>(move) * Constants::PLAYER_SPEED * profile.speedMultiplier;
//     const bool accelerating = move != 0 && (velocity.x == 0.0f || (velocity.x > 0.0f) == (targetSpeed > 0.0f));
//     const float horizontalRate = onGround
//         ? (accelerating ? Constants::PLAYER_GROUND_ACCELERATION : Constants::PLAYER_GROUND_DECELERATION)
//         : (accelerating ? Constants::PLAYER_AIR_ACCELERATION : Constants::PLAYER_AIR_DECELERATION);
//     velocity.x = approach(velocity.x, targetSpeed, horizontalRate * dt);
//     if (move != 0) facingDirection = move;
//     coyoteTimer = onGround ? Constants::PLAYER_COYOTE_TIME : std::max(0.0f, coyoteTimer - dt);
//     jumpBufferTimer = input.jump ? Constants::PLAYER_JUMP_BUFFER_TIME : std::max(0.0f, jumpBufferTimer - dt);
//     if (jumpBufferTimer > 0.0f && coyoteTimer > 0.0f) {
//         velocity.y = Constants::PLAYER_JUMP_SPEED * profile.jumpMultiplier;
//         onGround = false;
//         coyoteTimer = 0.0f;
//         jumpBufferTimer = 0.0f;
//     }
//     if (input.attack) attackTimer = 0.18f;
//     if (input.dodge && dodgeCooldownTimer <= 0.0f) {
//         dodgeTimer = Constants::PLAYER_DODGE_DURATION;
//         dodgeCooldownTimer = Constants::PLAYER_DODGE_COOLDOWN;
//     }
//     if (input.placeBlock) level.tryPlaceBlock(*this);
//     if (input.throwItem) level.throwItem(*this);

//     float gravityScale = profile.gravityMultiplier;
//     if (velocity.y > 0.0f) {
//         gravityScale *= Constants::PLAYER_FALL_GRAVITY_MULTIPLIER;
//     } else if (velocity.y < 0.0f && !input.jumpHeld) {
//         gravityScale *= Constants::PLAYER_JUMP_RELEASE_GRAVITY_MULTIPLIER;
//     } else if (std::abs(velocity.y) < Constants::PLAYER_APEX_SPEED_THRESHOLD) {
//         gravityScale *= Constants::PLAYER_APEX_GRAVITY_MULTIPLIER;
//     }
//     velocity.y = std::min(Constants::MAX_FALL_SPEED * profile.maxFallMultiplier,
//                           velocity.y + Constants::GRAVITY * gravityScale * dt);
//     velocity *= dt;
//     Collision::resolveTileCollision(*this, level.getTileMap());
//     velocity /= dt;

//     if (onGround) lastSafePosition = position;
//     if (position.y > level.getTileMap().heightPixels() + 260.0f) die(level, true);
// }

// void Player::draw(sf::RenderWindow& window, sf::Vector2f camera) const {
//     if (respawnTimer > 0.0f) return;
//     sf::RectangleShape body(size);
//     body.setPosition(position - camera);
//     const std::uint8_t alpha = isDodging() ? 88 : (hurtTimer > 0.0f ? 150 : 255);
//     body.setFillColor(sf::Color(color.r, color.g, color.b, alpha));
//     window.draw(body);

//     sf::RectangleShape eye({5.0f, 5.0f});
//     eye.setFillColor({248, 231, 200});
//     eye.setPosition(position - camera + sf::Vector2f(facingDirection > 0 ? 16.0f : 5.0f, 9.0f));
//     window.draw(eye);

//     if (isAttacking()) {
//         const auto box = attackBox();
//         sf::RectangleShape slash(box.size);
//         slash.setPosition(box.position - camera);
//         slash.setFillColor({245, 220, 130, 92});
//         window.draw(slash);
//     }
// }

// void Player::takeDamage(int damage, float knockback) {
//     if (respawnTimer > 0.0f) return;
//     if (firstHitGuardAvailable) {
//         firstHitGuardAvailable = false;
//         hurtTimer = 0.35f;
//         return;
//     }
//     Character::takeDamage(damage, knockback);
// }

// void Player::heal(int amount) {
//     health = std::min(maxHealth, health + amount);
// }

// void Player::restoreHunger(int amount) {
//     hunger = std::min<float>(Constants::PLAYER_MAX_HUNGER, hunger + amount);
// }

// void Player::die(Level& level, bool fell) {
//     if (respawnTimer > 0.0f) return;
//     Inventory stored = inventory.takeAll();
//     if (!stored.empty()) {
//         level.addTombstone(std::make_unique<Tombstone>(fell ? lastSafePosition : position, stored, id));
//     }
//     health = 0;
//     respawnTimer = 2.0f;
//     diedBefore = true;
//     recoveredTombstone = false;
// }

// bool Player::isRespawning() const {
//     return respawnTimer > 0.0f;
// }

// bool Player::isDodging() const {
//     return dodgeTimer > 0.0f && respawnTimer <= 0.0f;
// }

// bool Player::hasCollectedOwnTombstone() const {
//     return recoveredTombstone;
// }

// sf::FloatRect Player::attackBox() const {
//     const float x = facingDirection > 0 ? position.x + size.x - 2.0f : position.x - 34.0f;
//     return {{x, position.y + 8.0f}, {36.0f, 28.0f}};
// }

// bool Player::isAttacking() const {
//     return attackTimer > 0.0f && respawnTimer <= 0.0f;
// }

// int Player::getId() const { return id; }
// int Player::getFacingDirection() const { return facingDirection; }
// float Player::getHunger() const { return hunger; }
// float Player::getRespawnTimer() const { return respawnTimer; }
// const Player::Profile& Player::getProfile() const { return profile; }
// sf::Vector2f Player::getSpawn() const { return spawn; }
// bool Player::hasDiedBefore() const { return diedBefore; }

// void Player::setSpawn(sf::Vector2f spawnPosition) {
//     spawn = spawnPosition;
//     lastSafePosition = spawnPosition;
// }

// void Player::markTombstoneRecovered() {
//     recoveredTombstone = true;
// }
