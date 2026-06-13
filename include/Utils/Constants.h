#pragma once

namespace Constants {
    inline constexpr unsigned int WINDOW_WIDTH = 1280;
    inline constexpr unsigned int WINDOW_HEIGHT = 720;

    inline constexpr unsigned int FRAME_LIMIT = 60;
    inline constexpr float MAX_FRAME_TIME = 0.05f;

    inline constexpr int TILE_SIZE = 32;

    inline constexpr float GRAVITY = 1200.0f;
    inline constexpr float PLAYER_SPEED = 220.0f;
    inline constexpr float PLAYER_JUMP_SPEED = -480.0f;
    inline constexpr float MAX_PLAYER_DISTANCE = 900.0f;

    inline constexpr float DEBUG_PLAYER_WIDTH = 48.0f;
    inline constexpr float DEBUG_PLAYER_HEIGHT = 64.0f;

    inline constexpr int PLAYER_MAX_HEALTH = 5;
    inline constexpr int PLAYER_MAX_HUNGER = 100;
}