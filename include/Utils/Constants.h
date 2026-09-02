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

constexpr int PLAYER_MAX_HEALTH = 10;
constexpr int PLAYER_MAX_HUNGER = 100;
constexpr float HUNGER_LOSS_PER_SECOND = 1.0f;
constexpr float HUNGER_DAMAGE_INTERVAL = 1.0f;

constexpr int PATROL_ENEMY_HEALTH = 2;
constexpr int SHOOTER_ENEMY_HEALTH = 3;
constexpr int FLYING_ENEMY_HEALTH = 2;
constexpr int BOSS_HEALTH = 20;

constexpr float FLYING_ENEMY_SPEED = 72.0f;
constexpr float FLYING_ENEMY_WAVE_AMPLITUDE = 42.0f;
constexpr float FLYING_ENEMY_WAVE_FREQUENCY = 2.2f;
constexpr float FLYING_ENEMY_AGGRO_RANGE = 400.0f;
constexpr float FLYING_ENEMY_TRACK_RANGE = 150.0f;
constexpr float FLYING_ENEMY_PATROL_RANGE = 128.0f;
constexpr float FLYING_ENEMY_KNOCKBACK = 150.0f;
constexpr float FLYING_ENEMY_HURT_TIME = 0.25f;
constexpr float FLYING_ENEMY_DEATH_TIME = 1.0f;

constexpr float BOSS_MOVE_SPEED = 42.0f;
constexpr float BOSS_ENRAGED_SPEED_MULTIPLIER = 1.5f;
constexpr float BOSS_ENRAGED_HEALTH_RATIO = 0.5f;
constexpr float BOSS_TRACK_RANGE = 500.0f;
constexpr float BOSS_PHASE_ONE_ATTACK_COOLDOWN = 1.35f;
constexpr float BOSS_ENRAGED_ATTACK_COOLDOWN = 0.70f;
constexpr float BOSS_PROJECTILE_SPEED = 400.0f;
constexpr float BOSS_PROJECTILE_SPREAD_SPEED = 100.0f;
constexpr float BOSS_SHOCKWAVE_SPEED = 520.0f;
constexpr float BOSS_HURT_TIME = 0.30f;
constexpr float BOSS_DEATH_TIME = 1.0f;

constexpr int START_BLOCK_COUNT = 2;
constexpr unsigned WINDOW_WIDTH = 1280;
constexpr unsigned WINDOW_HEIGHT = 720;

constexpr int ITEM_ICON_SIZE = 15;
constexpr int COIN_FRAME_COUNT = 6;
constexpr int PROJECTILE_FRAME_COUNT = 4;

constexpr float EAT_ACTION_TIME = 0.45f;
constexpr float DROP_COIN_ACTION_TIME = 0.35f;
constexpr float PLACE_BLOCK_ACTION_TIME = 0.25f;

constexpr float FOOD_HUNGER_PERCENT[4] = {0.05f, 0.10f, 0.15f, 0.20f};

constexpr int MATCH_COIN_BONUS = 30;
constexpr float GOAL_WIDTH = 170.0f;
constexpr float GOAL_HEIGHT = 256.0f;
}
