#pragma once

#include <SFML/Graphics.hpp>

#include <Core/GameState.h>
#include <Utils/Constants.h>
#include <Core/AudioManager.h>

class Game{
public: 
    Game();
    void run();
private:
    GameState state = GameState::Menu;

    sf::RenderWindow window;
    AudioManager audio;

    void render();
};