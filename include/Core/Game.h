// Owns the window, game state, input, level, camera, and main loop.
#pragma once

#include "Core/AudioManager.h"
#include "Core/GameState.h"
#include "Core/InputManager.h"
#include "UI/HUD.h"
#include "UI/MenuScreen.h"
#include "World/Level.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    void restart();
    void update(float dt);
    void updateMenu();
    void updatePaused();
    void updateMapSelect();
    void updateDifficultySelect();
    void updateCharacterSelect();
    void updateShop();
    void updateCamera(float dt);
    void render();
    Player::Profile upgradedProfile(int profileIndex) const;
    bool profileUnlocked(int profileIndex) const;
    void moveProfileSelection(int player, int delta);

    sf::RenderWindow window;
    GameState state = GameState::Menu;
    AudioManager audio;
    InputManager input;
    Level level;
    HUD hud;
    MenuScreen menu;
    sf::Vector2f camera;
    int menuIndex = 0;
    int selectedProfiles[2] = {0, 1};
    int activeSelectPlayer = 0;
    int playerCount = 2;
    int selectedLevel = 0;
    int unlockedLevelCount = 1;
    int selectedDifficulty = 0;
    int walletCoins = 0;
    int shopIndex = 0;
    bool legendUnlocked = false;
    std::vector<int> profileLevels;
    float gameSpeed = 1.0f;
};
