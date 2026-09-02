#pragma once

/// State-pattern identifiers used to select application update and render behavior.
enum class GameState{
    Menu,
    Info,
    LevelSelect,
    DifficultySelect,
    Shop,
    CharacterSelect,
    Controls,
    Playing,
    Paused,
    GameOver,
    Victory,
};
