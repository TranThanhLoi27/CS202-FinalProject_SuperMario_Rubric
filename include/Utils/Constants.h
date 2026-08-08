// Gameplay constants shared by every module.
#pragma once

namespace Constants {
constexpr int TILE_SIZE = 32;

constexpr float GRAVITY = 1600.0f;
constexpr float PLAYER_SPEED = 245.0f;
constexpr float PLAYER_GROUND_ACCELERATION = 2400.0f;
constexpr float PLAYER_AIR_ACCELERATION = 1500.0f;
constexpr float PLAYER_GROUND_DECELERATION = 3000.0f;
constexpr float PLAYER_AIR_DECELERATION = 950.0f;
constexpr float PLAYER_JUMP_SPEED = -610.0f;
constexpr float PLAYER_JUMP_BUFFER_TIME = 0.12f;
constexpr float PLAYER_COYOTE_TIME = 0.09f;
constexpr float PLAYER_APEX_SPEED_THRESHOLD = 90.0f;
constexpr float PLAYER_APEX_GRAVITY_MULTIPLIER = 0.78f;
constexpr float PLAYER_FALL_GRAVITY_MULTIPLIER = 1.12f;
constexpr float PLAYER_JUMP_RELEASE_GRAVITY_MULTIPLIER = 1.7f;
constexpr float MAX_PLAYER_DISTANCE = 1280.0f;
constexpr float MAX_FALL_SPEED = 980.0f;
constexpr float PLAYER_DODGE_DURATION = 0.45f;
constexpr float PLAYER_DODGE_COOLDOWN = 2.0f;

constexpr int PLAYER_MAX_HEALTH = 5;
constexpr int PLAYER_MAX_HUNGER = 100;
constexpr float HUNGER_LOSS_PER_SECOND = 1.0f;
constexpr float HUNGER_DAMAGE_INTERVAL = 1.0f;

constexpr int PATROL_ENEMY_HEALTH = 2;
constexpr int SHOOTER_ENEMY_HEALTH = 3;
constexpr int BOSS_HEALTH = 20;

constexpr int START_BLOCK_COUNT = 3;
constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEIGHT = 720;
}
