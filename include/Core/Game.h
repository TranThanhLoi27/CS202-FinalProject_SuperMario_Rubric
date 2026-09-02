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

    /// Initializes fixed-size progression and achievement collections.
    SaveData() {
        profileLevels.resize(5, 0); // 5 characters
        achievements.resize(4, 0); // 4 achievements
    }

    /// Serializes persistent progression to a binary file.
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

    /// Loads persistent progression from a binary file after sanity checks.
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

    /// Restores all persistent progression fields to their initial values.
    void reset() {
        walletCoins = 0;
        unlockedLevelCount = 1;
        legendUnlocked = false;
        profileLevels.assign(5, 0);
        achievements.assign(4, false);
    }
};

/// Facade that owns the window and coordinates all application subsystems.
class Game {
public:
    /// Creates the window and initializes every application subsystem.
    Game();
    /// Saves progression and disconnects non-owning event observers.
    ~Game();
    /// Runs the event, update, audio, and render loop until the window closes.
    void run();

private:
    /// Loads and registers every required texture.
    void loadTexture();
    /// Loads every optional sound effect used by the game.
    void loadAudio();
    /// Builds and registers animation metadata for each character sprite set.
    void registerCharacterSprites();
    /// Reloads the selected level and enters the Playing state.
    void restart();
    /// Updates the active game state for one frame.
    void update(float dt);
    /// Handles main-menu and submenu interaction.
    void updateMenu();
    /// Handles pause-menu interaction.
    void updatePaused();
    /// Handles key-binding selection and key capture.
    void updateControls(float dt);
    /// Handles map selection and locked-level checks.
    void updateMapSelect();
    /// Handles difficulty selection.
    void updateDifficultySelect();
    /// Handles one-player or two-player profile selection.
    void updateCharacterSelect();
    /// Handles upgrades, purchases, and shop navigation.
    void updateShop();
    /// Returns the logical menu item beneath a world-space mouse position.
    int menuItemAt(sf::Vector2f mousePosition) const;
    /// Returns the clicked menu item for the current frame, if any.
    int clickedMenuItem() const;
    /// Updates the shared camera target and world clamping.
    void updateCamera(float dt);
    /// Draws the world, HUD, menus, and transient notifications.
    void render();
    /// Returns a profile with purchased upgrades applied.
    Player::Profile upgradedProfile(int profileIndex) const;
    /// Reports whether the requested profile can currently be selected.
    bool profileUnlocked(int profileIndex) const;
    /// Moves a player's profile index while skipping unavailable profiles.
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

    /// Copies runtime progression into SaveData and writes it to disk.
    void saveGame();
    /// Restores saved progression or initializes safe defaults.
    void loadGame();
    /// Clears saved and in-memory progression.
    void resetGame();
};
