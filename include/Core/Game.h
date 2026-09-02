// Owns the window, game state, input, level, camera, and main loop.
#pragma once

#include "Core/AudioManager.h"
#include "Core/AssetManager.h"
#include "Core/GameState.h"
#include "Core/InputManager.h"
#include "Graphics/Animation.h"
#include "UI/HUD.h"
#include "UI/MenuScreen.h"
#include "UI/SettingsScreen.h"
#include "World/Level.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class Game {
public:
    Game();
    void run();

private:
    void loadTexture();
    void registerCharacterSprites();
    void restart();
    void update(float dt);
    void updateUiHover();
    void handleMouseClick(sf::Vector2f mousePosition);
    void handleMenuClick(int buttonIndex);
    void handleShopClick(int buttonIndex);
    void openSettings(GameState returnState);
    void updateCamera(float dt);
    void render();
    Player::Profile upgradedProfile(int profileIndex) const;
    bool profileUnlocked(int profileIndex) const;

    sf::RenderWindow window;
    AssetManager assets;
    GameState state = GameState::Menu;
    AudioManager audio;
    InputManager input;
    Level level;
    HUD hud;
    MenuScreen menu;
    SettingsScreen settings;
    GameState settingsReturnState = GameState::Menu;
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
