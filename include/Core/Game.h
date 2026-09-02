// Owns the window, game state, input, level, camera, and main loop.
#pragma once

#include "Core/AudioManager.h"
#include "Core/AssetManager.h"
#include "Core/GameState.h"
#include "Core/InputManager.h"
#include "Graphics/Animation.h"
#include "UI/HUD.h"
#include "UI/MenuScreen.h"
#include "World/Level.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>

struct SaveData {
    int walletCoins = 0;
    int unlockedLevelCount = 1;
    bool legendUnlocked = false;
    std::vector<int> profileLevels;
    std::vector<char> achievements; // First kill, 5 kills, no damage run, boss defeated (char instead of bool for serialization)

    SaveData() {
        profileLevels.resize(5, 0); // 5 characters
        achievements.resize(4, 0); // 4 achievements
    }

    void saveToFile(const std::string& filename = "data.txt") {
        std::ofstream file(filename, std::ios::binary);
        if (!file) return;

        try {
            // Write simple data types
            file.write(reinterpret_cast<const char*>(&walletCoins), sizeof(walletCoins));
            file.write(reinterpret_cast<const char*>(&unlockedLevelCount), sizeof(unlockedLevelCount));
            file.write(reinterpret_cast<const char*>(&legendUnlocked), sizeof(legendUnlocked));

            // Write profileLevels vector
            size_t profileSize = profileLevels.size();
            file.write(reinterpret_cast<const char*>(&profileSize), sizeof(profileSize));
            if (profileSize > 0) {
                file.write(reinterpret_cast<const char*>(profileLevels.data()), profileSize * sizeof(int));
            }

            // Write achievements vector
            size_t achievementSize = achievements.size();
            file.write(reinterpret_cast<const char*>(&achievementSize), sizeof(achievementSize));
            if (achievementSize > 0) {
                file.write(reinterpret_cast<const char*>(achievements.data()), achievementSize * sizeof(char));
            }

            file.close();
        } catch (...) {
            file.close();
        }
    }

    bool loadFromFile(const std::string& filename = "data.txt") {
        std::ifstream file(filename, std::ios::binary);
        if (!file) return false;

        try {
            // Read simple data types
            file.read(reinterpret_cast<char*>(&walletCoins), sizeof(walletCoins));
            file.read(reinterpret_cast<char*>(&unlockedLevelCount), sizeof(unlockedLevelCount));
            file.read(reinterpret_cast<char*>(&legendUnlocked), sizeof(legendUnlocked));

            // Read profileLevels vector
            size_t profileSize;
            file.read(reinterpret_cast<char*>(&profileSize), sizeof(profileSize));
            if (profileSize > 100) return false; // Sanity check
            profileLevels.resize(profileSize);
            file.read(reinterpret_cast<char*>(profileLevels.data()), profileSize * sizeof(int));

            // Read achievements vector
            size_t achievementSize;
            file.read(reinterpret_cast<char*>(&achievementSize), sizeof(achievementSize));
            if (achievementSize > 100) return false; // Sanity check
            achievements.resize(achievementSize);
            file.read(reinterpret_cast<char*>(achievements.data()), achievementSize * sizeof(char));

            file.close();
            return true;
        } catch (...) {
            file.close();
            return false;
        }
    }

    void reset() {
        walletCoins = 0;
        unlockedLevelCount = 1;
        legendUnlocked = false;
        profileLevels.assign(5, 0);
        achievements.assign(4, false);
    }
};

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void loadTexture();
    void loadAudio();
    void registerCharacterSprites();
    void restart();
    void update(float dt);
    void updateMenu();
    void updatePaused();
    void updateControls(float dt);
    void updateMapSelect();
    void updateDifficultySelect();
    void updateCharacterSelect();
    void updateShop();
    int menuItemAt(sf::Vector2f mousePosition) const;
    int clickedMenuItem() const;
    void updateCamera(float dt);
    void render();
    Player::Profile upgradedProfile(int profileIndex) const;
    bool profileUnlocked(int profileIndex) const;
    void moveProfileSelection(int player, int delta);

    sf::RenderWindow window;
    AssetManager assets;
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
    int pauseMenuIndex = 0;
    int controlsPlayerIndex = 0;
    int controlsActionIndex = 0;
    bool rebinding = false;
    std::string rebindWarning;
    float rebindWarningTimer = 0.0f;
    sf::Vector2i lastMousePos{-1, -1};
    sf::Vector2f leftClickPosition{};
    bool leftClickPending = false;
    int hoveredMenuItem = -1;

    // Achievement system
    std::string currentAchievementText;
    float achievementDisplayTimer = 0.0f;
    std::vector<char> previousAchievements;

    SaveData saveData;

    void saveGame();
    void loadGame();
    void resetGame();
};
