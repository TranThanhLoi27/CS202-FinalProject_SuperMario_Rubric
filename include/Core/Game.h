#pragma once

#include <SFML/Graphics.hpp>

#include <Core/GameState.h>
#include <Utils/Constants.h>
#include <World/Level.h>
#include <Core/Camera.h>
#include <Core/AssetManager.h>
#include <Core/AudioManager.h>

class Game{
public: 
    Game();
    void run();
private:
    GameState state = GameState::Menu;

    sf::RenderWindow window;
    sf::Vector2f camera;
    AssetManager assets;
    AudioManager audio;
    Level level;

    void loadAsset();
    void render();
};