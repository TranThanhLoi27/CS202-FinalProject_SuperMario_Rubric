// Runs the application loop and coordinates state transitions between modules.
#include "Core/Game.h"

#include "Core/Camera.h"
#include "Entities/DroppedItem.h"
#include "Entities/Inventory.h"
#include "Entities/Projectile.h"
#include "Entities/Tombstone.h"
#include "Graphics/CharacterSprites.h"
#include "Graphics/EnemySprites.h"
#include "UI/MenuScreen.h"
#include "Utils/Constants.h"

#include <algorithm>
#include <array>
#include <optional>
#include <stdexcept>

namespace {
struct LevelOption {
    const char* name;
    const char* path;
    bool boss;
};

struct DifficultyOption {
    const char* name;
    int enemyHealthBonus;
};

const std::array<LevelOption, 4> LEVELS = {{
    {"Forest Trail", "assets/levels/level1.txt", false},
    {"Stone Bridge", "assets/levels/level2.txt", false},
    {"Spike Valley", "assets/levels/level3.txt", false},
    {"Boss Lair", "assets/levels/boss.txt", true},
}};

const std::array<DifficultyOption, 3> DIFFICULTIES = {{
    {"Easy", 0},
    {"Medium", 1},
    {"Hard", 2},
}};

constexpr int LEGEND_PRICE = 80;
constexpr int UPGRADE_PRICE = 20;
}

Game::Game()
    : window(sf::VideoMode({Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT}), "2D Co-op Combat Platformer") {
    window.setFramerateLimit(120);
    window.setVerticalSyncEnabled(true);
    if (!assets.LoadFont("roboto", "assets/fonts/roboto.ttf")) {
        throw std::runtime_error("Could not load assets/fonts/roboto.ttf");
    }
    HUD::setFont(assets.font("roboto"));
    loadTexture();
    MenuScreen::setSelectorTextures(assets.texture("selector1"), assets.texture("selector2"));
    Level::setTextures(assets.texture("solid"), assets.texture("goal"), assets.texture("spike"), assets.texture("fairy"));
    HUD::setTextures(assets.texture("heart"), assets.texture("meat"), assets.texture("food"),
                      assets.texture("coin"), assets.texture("solid"));
    EnemyTextures::setTextures(assets.texture("patrol"), assets.texture("shooter"), 
                               assets.texture("flyingIdle"), assets.texture("flyingHurt"), assets.texture("flyingDie"),
                               assets.texture("bossIdle"), assets.texture("bossWalk"), assets.texture("bossHurt"),
                               assets.texture("bossDie"), assets.texture("bossAttack"), assets.texture("boss_projectile"));
    Projectile::setTextures(assets.texture("shooter_projectile"), assets.texture("boss_projectile"));
    DroppedItem::setTextures(assets.texture("food"), assets.texture("coin"), assets.texture("solid"));
    registerCharacterSprites();
    Tombstone::setTexture(assets.texture("tombstone"));

    profileLevels.assign(Player::profiles().size(), 0);
    level.loadDefault();
    audio.setMasterVolume(70.0f);
    audio.playMusic("assets/audio/background.ogg");

    // Initialize achievement tracking
    previousAchievements.resize(4, 0);
    achievementDisplayTimer = 0.0f;

    // Try to load saved game data
    loadGame();
}

Game::~Game() {
    // Auto-save when closing the game
    saveGame();
}

void Game::loadTexture(){
    assets.LoadTexture("heart", "assets/textures/heart.png");
    assets.LoadTexture("meat", "assets/textures/meat.png");
    assets.LoadTexture("food", "assets/textures/food.png");
    assets.LoadTexture("coin", "assets/textures/coin.png");
    assets.LoadTexture("rush", "assets/textures/rush.png");
    if (!assets.LoadTexture("background", "assets/textures/background.png") ||
        !assets.LoadTexture("selector2", "assets/textures/selector1.png") ||
        !assets.LoadTexture("selector1", "assets/textures/selector2.png") ||
        !assets.LoadTexture("fairy", "assets/textures/fairy.png") ||
        !assets.LoadTexture("solid", "assets/textures/solid.png") ||
        !assets.LoadTexture("goal", "assets/textures/goal.png") ||
        !assets.LoadTexture("spike", "assets/textures/spike.png") ||
        !assets.LoadTexture("ori", "assets/textures/ori.png") ||
        (!assets.LoadTexture("patrol", "assets/textures/slime.png") && !assets.LoadTexture("patrol", "assets/textures/patrol.png")) ||
        !assets.LoadTexture("shooter", "assets/textures/shooter.png") ||
        !assets.LoadTexture("bossIdle", "assets/textures/boss/Golem_idle.png") ||
        !assets.LoadTexture("bossWalk", "assets/textures/boss/Golem_walk.png") ||
        !assets.LoadTexture("bossHurt", "assets/textures/boss/Golem_hurt.png") ||
        !assets.LoadTexture("bossDie", "assets/textures/boss/Golem_die.png") ||
        !assets.LoadTexture("bossAttack", "assets/textures/boss/Golem_attack.png") ||
        !assets.LoadTexture("boss_projectile", "assets/textures/boss_projectile.png") ||
        !assets.LoadTexture("shooter_projectile", "assets/textures/shooter_eneny_projectile.png") ||
        !assets.LoadTexture("tombstone", "assets/textures/tombstone.png")) {
        throw std::runtime_error("Could not load required textures");
    }
    if ((!assets.LoadTexture("flyingIdle", "assets/textures/bat/Bat-Run.png") &&
         !assets.LoadTexture("flyingIdle", "assets/textures/bat/Bat-IdleFly.png")) ||
        !assets.LoadTexture("flyingHurt", "assets/textures/bat/Bat-Hurt.png") ||
        !assets.LoadTexture("flyingDie", "assets/textures/bat/Bat-Die.png")) {
        throw std::runtime_error("Could not load bat textures for flying enemy");
    }
}

void Game::registerCharacterSprites() {
    constexpr int frameWidth = 420;
    constexpr int frameHeight = 280;
    constexpr int runStart = 0;
    constexpr int runCount = 3;
    constexpr int jumpFrame = 3;
    constexpr int idleFrame = 4;
    constexpr int attackFrame = 5;

    CharacterSpriteSet ori;
    ori.texture = &assets.texture("ori");
    ori.frameWidth = frameWidth;
    ori.frameHeight = frameHeight;
    ori.run = Animation(ori.frameRect(runStart), runCount, 0.10f);
    ori.idle = Animation(ori.frameRect(idleFrame), 1, 0.10f);
    ori.jump = Animation(ori.frameRect(jumpFrame), 1, 0.10f);
    ori.attack = Animation(ori.frameRect(attackFrame), 1, 0.08f);
    ori.runSpeedThreshold = 5.0f;
    CharacterSprites::registerSet("ori", std::move(ori));

    CharacterSpriteSet rush;
    rush.texture = &assets.texture("rush");
    rush.frameWidth = frameWidth;
    rush.frameHeight = frameHeight;
    rush.run = Animation(rush.frameRect(runStart), runCount, 0.10f);
    rush.idle = Animation(rush.frameRect(idleFrame), 1, 0.10f);
    rush.jump = Animation(rush.frameRect(jumpFrame), 1, 0.10f);
    rush.attack = Animation(rush.frameRect(attackFrame), 1, 0.08f);
    rush.runSpeedThreshold = 5.0f;
    CharacterSprites::registerSet("rush", std::move(rush));
}

void Game::run() {
    sf::Clock clock;
    input.update(window);
    while (window.isOpen()) {
        float wheelDelta = 0.0f;
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
                if (mouseWheel->wheel == sf::Mouse::Wheel::Vertical) {
                    wheelDelta += mouseWheel->delta;
                }
            }
        }

        input.update(window);
        if (wheelDelta != 0.0f) {
            input.setMouseWheelDelta(wheelDelta);
        }

        audio.update();
        const float dt = std::min(clock.restart().asSeconds(), 1.0f / 30.0f);
        update(dt);
        render();
    }
}

void Game::restart() {
    const Player::Profile p1 = upgradedProfile(selectedProfiles[0]);
    const Player::Profile p2 = upgradedProfile(selectedProfiles[1]);
    const auto& option = LEVELS[static_cast<std::size_t>(selectedLevel)];
    const int healthBonus = DIFFICULTIES[static_cast<std::size_t>(selectedDifficulty)].enemyHealthBonus;
    const std::string path(option.path);
    if (!level.loadLevel(path, p1, p2, playerCount, healthBonus)) {
        level.loadLevel("../" + path, p1, p2, playerCount, healthBonus);
    }
    camera = {};
    state = GameState::Playing;
}

void Game::update(float dt) {
    if (state == GameState::Menu || state == GameState::Info || state == GameState::LevelSelect ||
        state == GameState::DifficultySelect || state == GameState::Shop || state == GameState::CharacterSelect) {
        updateMenu();
        return;
    }

    // Update achievement display timer
    if (achievementDisplayTimer > 0.0f) {
        achievementDisplayTimer -= dt;
    }

    // Check for new achievements
    const auto& currentAchievements = level.getAchievements();
    for (size_t i = 0; i < currentAchievements.size(); ++i) {
        if (currentAchievements[i] && previousAchievements[i] == 0) {
            // New achievement unlocked!
            const char* achievementNames[] = {"First Kill", "5 Kills", "No Damage Run", "Boss Defeated"};
            currentAchievementText = achievementNames[i];
            achievementDisplayTimer = 3.0f; // Show for 3 seconds
            previousAchievements[i] = 1;

            // Trigger fireworks effect
            const auto& players = level.getPlayers();
            if (!players.empty() && players[0]) {
                level.getFairies().createFireworks(players[0]->position, 20);
            }
        }
    }

    const InputState& p1Input = input.getPlayer1Input();
    if (p1Input.menuConfirm &&
        (state == GameState::Victory || state == GameState::GameOver)) {
        restart();
    }
    if (p1Input.menuBack &&
        (state == GameState::Victory || state == GameState::GameOver)) {
        state = GameState::Menu;
    }
    if (p1Input.menuBack && state == GameState::Playing) {
        state = GameState::Paused;
        pauseMenuIndex = 0;
    }
    if (p1Input.pause &&
        (state == GameState::Playing || state == GameState::Paused)) {
        state = state == GameState::Playing ? GameState::Paused : GameState::Playing;
        if (state == GameState::Paused) pauseMenuIndex = 0;
    }
    if (state == GameState::Paused) {
        updatePaused();
        return;
    }
    if (state == GameState::Controls) {
        updateControls(dt);
        return;
    }
    if (state != GameState::Playing) return;

    InputState p1 = input.getPlayer1Input();
    InputState p2 = input.getPlayer2Input();
    float remaining = dt * gameSpeed;
    constexpr float physicsStep = 1.0f / 120.0f;
    while (remaining > 0.0f) {
        const float stepDt = std::min(physicsStep, remaining);
        level.update(stepDt, p1, p2);
        if (state == GameState::Playing) {
            level.checkAchievements();
        }
        updateCamera(stepDt);
        p1.jump = p1.attack = p1.dodge = p1.useItem = false;
        p1.slotPrev = p1.slotNext = false;
        p1.slotSelect = -1;
        p2.jump = p2.attack = p2.dodge = p2.useItem = false;
        p2.slotPrev = p2.slotNext = false;
        p2.slotSelect = -1;
        remaining -= stepDt;
    }
    if (level.hasWon()) {
        for (auto& player : level.getPlayers()) {
            player->getInventory().addToSlot(COIN_SLOT_INDEX, Constants::MATCH_COIN_BONUS);
        }
        walletCoins += level.collectedCoins();
        unlockedLevelCount = std::max(unlockedLevelCount, std::min(static_cast<int>(LEVELS.size()), selectedLevel + 2));
        state = GameState::Victory;
    }
    if (level.allDead()) state = GameState::GameOver;
}

void Game::updateMenu() {
    const InputState& p1Input = input.getPlayer1Input();

    // Handle mouse wheel viewport scrolling
    if (input.getMouseWheelDelta() != 0.0f) {
        const int delta = input.getMouseWheelDelta() > 0 ? -1 : 1;
        const int maxVisible = MenuScreen::getMaxVisibleItems(state);
        const int totalItems = MenuScreen::getTotalItems(state, legendUnlocked, profileLevels);
        int newOffset = MenuScreen::getViewportOffset() + delta;
        newOffset = std::max(0, std::min(newOffset, totalItems - maxVisible));
        MenuScreen::setViewportOffset(newOffset);
        input.resetMouseWheelDelta();
    }

    const sf::Vector2i currentMouse = input.getMousePosition();
    const int itemUnderMouse = MenuScreen::getMenuItemUnderMouse(state, currentMouse, menuIndex,
                                                                 selectedLevel, unlockedLevelCount, selectedDifficulty,
                                                                 walletCoins, shopIndex, legendUnlocked, profileLevels,
                                                                 pauseMenuIndex, controlsPlayerIndex, controlsActionIndex);
    if (itemUnderMouse >= 0 && itemUnderMouse < 7) {
        menuIndex = itemUnderMouse;
    }

    if (state == GameState::Info) {
        if (p1Input.menuConfirm || p1Input.menuBack || input.mousePressed(sf::Mouse::Button::Left)) {
            state = GameState::Menu;
        }
        return;
    }

    if (state == GameState::LevelSelect) {
        updateMapSelect();
        return;
    }

    if (state == GameState::DifficultySelect) {
        updateDifficultySelect();
        return;
    }

    if (state == GameState::Shop) {
        updateShop();
        return;
    }

    if (state == GameState::CharacterSelect) {
        updateCharacterSelect();
        return;
    }

    if (p1Input.info) {
        state = GameState::Info;
        return;
    }

    if (input.mousePressed(sf::Mouse::Button::Left) && itemUnderMouse >= 0) {
        menuIndex = itemUnderMouse;
        if (menuIndex == 0 || menuIndex == 1) {
            playerCount = menuIndex + 1;
            activeSelectPlayer = 0;
            state = GameState::LevelSelect;
            MenuScreen::setViewportOffset(0);
        } else if (menuIndex == 2) {
            state = GameState::DifficultySelect;
            MenuScreen::setViewportOffset(0);
        } else if (menuIndex == 3) {
            state = GameState::Shop;
            MenuScreen::setViewportOffset(0);
        } else if (menuIndex == 4) {
            state = GameState::Info;
            MenuScreen::setViewportOffset(0);
        } else if (menuIndex == 5) {
            resetGame();
        } else if (menuIndex == 6) {
            window.close();
        }
    }
}

void Game::updatePaused() {
    constexpr int pauseItemCount = 3;

    // Handle mouse wheel viewport scrolling
    if (input.getMouseWheelDelta() != 0.0f) {
        const int delta = input.getMouseWheelDelta() > 0 ? -1 : 1;
        const int maxVisible = MenuScreen::getMaxVisibleItems(state);
        const int totalItems = MenuScreen::getTotalItems(state, legendUnlocked, profileLevels);
        int newOffset = MenuScreen::getViewportOffset() + delta;
        newOffset = std::max(0, std::min(newOffset, totalItems - maxVisible));
        MenuScreen::setViewportOffset(newOffset);
        input.resetMouseWheelDelta();
    }

    const sf::Vector2i currentMouse = input.getMousePosition();
    const int itemUnderMouse = MenuScreen::getMenuItemUnderMouse(state, currentMouse, menuIndex,
                                                                 selectedLevel, unlockedLevelCount, selectedDifficulty,
                                                                 walletCoins, shopIndex, legendUnlocked, profileLevels,
                                                                 pauseMenuIndex, controlsPlayerIndex, controlsActionIndex);
    if (itemUnderMouse >= 0 && itemUnderMouse < pauseItemCount) {
        pauseMenuIndex = itemUnderMouse;
    }

    if (input.getPlayer1Input().menuBack) {
        state = GameState::Playing;
        return;
    }

    if (input.mousePressed(sf::Mouse::Button::Left) && itemUnderMouse >= 0 && itemUnderMouse < pauseItemCount) {
        pauseMenuIndex = itemUnderMouse;
        if (pauseMenuIndex == 0) state = GameState::Playing;
        else if (pauseMenuIndex == 1) {
            state = GameState::Controls;
            controlsPlayerIndex = 0;
            controlsActionIndex = 0;
            rebinding = false;
            rebindWarning.clear();
            MenuScreen::setViewportOffset(0);
        } else if (pauseMenuIndex == 2) {
            state = GameState::Menu;
            MenuScreen::setViewportOffset(0);
        }
    }
}

void Game::updateControls(float dt) {
    if (rebindWarningTimer > 0.0f) {
        rebindWarningTimer -= dt;
        if (rebindWarningTimer <= 0.0f) rebindWarning.clear();
    }

    // Handle mouse wheel viewport scrolling
    if (input.getMouseWheelDelta() != 0.0f) {
        const int delta = input.getMouseWheelDelta() > 0 ? -1 : 1;
        const int maxVisible = MenuScreen::getMaxVisibleItems(state);
        const int totalItems = MenuScreen::getTotalItems(state, legendUnlocked, profileLevels);
        int newOffset = MenuScreen::getViewportOffset() + delta;
        newOffset = std::max(0, std::min(newOffset, totalItems - maxVisible));
        MenuScreen::setViewportOffset(newOffset);
        input.resetMouseWheelDelta();
    }

    const sf::Vector2i currentMouse = input.getMousePosition();
    const int itemUnderMouse = MenuScreen::getMenuItemUnderMouse(state, currentMouse, menuIndex,
                                                                 selectedLevel, unlockedLevelCount, selectedDifficulty,
                                                                 walletCoins, shopIndex, legendUnlocked, profileLevels,
                                                                 pauseMenuIndex, controlsPlayerIndex, controlsActionIndex);
    if (itemUnderMouse >= 0 && itemUnderMouse < InputManager::ActionCount) {
        controlsActionIndex = itemUnderMouse;
    }

    if (rebinding) {
        if (input.getPlayer1Input().menuBack) {
            rebinding = false;
            rebindWarning.clear();
            return;
        }
        const sf::Keyboard::Key newKey = input.pollNewKey();
        if (newKey != sf::Keyboard::Key::Unknown) {
            if (input.isKeyAssigned(newKey)) {
                rebindWarning = "Key " + InputManager::getKeyName(newKey) + " already in use!";
                rebindWarningTimer = 2.0f;
            } else {
                input.rebindKey(controlsPlayerIndex, controlsActionIndex, newKey);
                rebinding = false;
                rebindWarning.clear();
            }
        }
        return;
    }

    if (input.getPlayer1Input().menuBack) {
        state = GameState::Paused;
        MenuScreen::setViewportOffset(0);
        return;
    }

    if (input.mousePressed(sf::Mouse::Button::Left)) {
        if (itemUnderMouse == -10) {
            controlsPlayerIndex = 0;
            return;
        }
        if (itemUnderMouse == -11) {
            controlsPlayerIndex = 1;
            return;
        }
        if (itemUnderMouse >= 0 && itemUnderMouse < InputManager::ActionCount) {
            controlsActionIndex = itemUnderMouse;
            rebinding = true;
            rebindWarning.clear();
        }
    }
}

void Game::updateMapSelect() {
    if (input.getMouseWheelDelta() != 0.0f) {
        const int delta = input.getMouseWheelDelta() > 0 ? -1 : 1;
        const int maxVisible = MenuScreen::getMaxVisibleItems(state);
        const int totalItems = MenuScreen::getTotalItems(state, legendUnlocked, profileLevels);
        int newOffset = MenuScreen::getViewportOffset() + delta;
        newOffset = std::max(0, std::min(newOffset, totalItems - maxVisible));
        MenuScreen::setViewportOffset(newOffset);
        input.resetMouseWheelDelta();
    }

    const sf::Vector2i currentMouse = input.getMousePosition();
    const int itemUnderMouse = MenuScreen::getMenuItemUnderMouse(state, currentMouse, menuIndex,
                                                                 selectedLevel, unlockedLevelCount, selectedDifficulty,
                                                                 walletCoins, shopIndex, legendUnlocked, profileLevels,
                                                                 pauseMenuIndex, controlsPlayerIndex, controlsActionIndex);
    if (itemUnderMouse >= 0 && itemUnderMouse < static_cast<int>(LEVELS.size())) {
        selectedLevel = itemUnderMouse;
    }

    if (input.getPlayer1Input().menuBack) {
        state = GameState::Menu;
        MenuScreen::setViewportOffset(0);
        return;
    }

    if (input.mousePressed(sf::Mouse::Button::Left) && itemUnderMouse >= 0) {
        if (itemUnderMouse < unlockedLevelCount) {
            selectedLevel = itemUnderMouse;
            activeSelectPlayer = 0;
            state = GameState::CharacterSelect;
            MenuScreen::setViewportOffset(0);
        }
    }
}

void Game::updateDifficultySelect() {
    if (input.getMouseWheelDelta() != 0.0f) {
        const int delta = input.getMouseWheelDelta() > 0 ? -1 : 1;
        const int maxVisible = MenuScreen::getMaxVisibleItems(state);
        const int totalItems = MenuScreen::getTotalItems(state, legendUnlocked, profileLevels);
        int newOffset = MenuScreen::getViewportOffset() + delta;
        newOffset = std::max(0, std::min(newOffset, totalItems - maxVisible));
        MenuScreen::setViewportOffset(newOffset);
        input.resetMouseWheelDelta();
    }

    const sf::Vector2i currentMouse = input.getMousePosition();
    const int itemUnderMouse = MenuScreen::getMenuItemUnderMouse(state, currentMouse, menuIndex,
                                                                 selectedLevel, unlockedLevelCount, selectedDifficulty,
                                                                 walletCoins, shopIndex, legendUnlocked, profileLevels,
                                                                 pauseMenuIndex, controlsPlayerIndex, controlsActionIndex);
    if (itemUnderMouse >= 0 && itemUnderMouse < 3) {
        selectedDifficulty = itemUnderMouse;
    }

    if (input.getPlayer1Input().menuBack) {
        state = GameState::Menu;
        MenuScreen::setViewportOffset(0);
        return;
    }

    if (input.mousePressed(sf::Mouse::Button::Left) && itemUnderMouse >= 0 && itemUnderMouse < 3) {
        selectedDifficulty = itemUnderMouse;
        state = GameState::Menu;
        MenuScreen::setViewportOffset(0);
    }
}

void Game::updateCharacterSelect() {
    if (input.getMouseWheelDelta() != 0.0f) {
        const int delta = input.getMouseWheelDelta() > 0 ? -1 : 1;
        const int maxVisible = MenuScreen::getMaxVisibleItems(state);
        const int totalItems = MenuScreen::getTotalItems(state, legendUnlocked, profileLevels);
        int newOffset = MenuScreen::getViewportOffset() + delta;
        newOffset = std::max(0, std::min(newOffset, totalItems - maxVisible));
        MenuScreen::setViewportOffset(newOffset);
        input.resetMouseWheelDelta();
    }

    const sf::Vector2i currentMouse = input.getMousePosition();
    const int itemUnderMouse = MenuScreen::getMenuItemUnderMouse(state, currentMouse, menuIndex,
                                                                 selectedLevel, unlockedLevelCount, selectedDifficulty,
                                                                 walletCoins, shopIndex, legendUnlocked, profileLevels,
                                                                 pauseMenuIndex, controlsPlayerIndex, controlsActionIndex);
    if (itemUnderMouse >= 0 && itemUnderMouse < static_cast<int>(Player::profiles().size())) {
        if (profileUnlocked(itemUnderMouse)) {
            selectedProfiles[activeSelectPlayer] = itemUnderMouse;
        }
    }

    if (input.getPlayer1Input().menuBack) {
        if (playerCount == 2 && activeSelectPlayer == 1) {
            activeSelectPlayer = 0; // Go back to Player 1 pick
        } else {
            state = GameState::LevelSelect;
            MenuScreen::setViewportOffset(0);
        }
        return;
    }

    if (input.mousePressed(sf::Mouse::Button::Left) && itemUnderMouse >= 0 && itemUnderMouse < static_cast<int>(Player::profiles().size())) {
        if (profileUnlocked(itemUnderMouse)) {
            selectedProfiles[activeSelectPlayer] = itemUnderMouse;
            if (playerCount == 2 && activeSelectPlayer == 0) {
                activeSelectPlayer = 1; // 1st click picked P1 character -> switch to P2
            } else {
                restart(); // 2nd click (or 1-player click) starts game!
            }
        }
    }
}

void Game::updateShop() {
    const int count = static_cast<int>(Player::profiles().size());
    const int rows = count + 1;

    // Handle mouse wheel viewport scrolling
    if (input.getMouseWheelDelta() != 0.0f) {
        const int delta = input.getMouseWheelDelta() > 0 ? -1 : 1;
        const int maxVisible = MenuScreen::getMaxVisibleItems(state);
        const int totalItems = MenuScreen::getTotalItems(state, legendUnlocked, profileLevels);
        int newOffset = MenuScreen::getViewportOffset() + delta;
        newOffset = std::max(0, std::min(newOffset, totalItems - maxVisible));
        MenuScreen::setViewportOffset(newOffset);
        input.resetMouseWheelDelta();
    }

    const sf::Vector2i currentMouse = input.getMousePosition();
    const int itemUnderMouse = MenuScreen::getMenuItemUnderMouse(state, currentMouse, menuIndex,
                                                                 selectedLevel, unlockedLevelCount, selectedDifficulty,
                                                                 walletCoins, shopIndex, legendUnlocked, profileLevels,
                                                                 pauseMenuIndex, controlsPlayerIndex, controlsActionIndex);
    if (itemUnderMouse >= 0 && itemUnderMouse < rows) {
        shopIndex = itemUnderMouse;
    }

    if (input.getPlayer1Input().menuBack) {
        state = GameState::Menu;
        MenuScreen::setViewportOffset(0);
        return;
    }

    if (input.mousePressed(sf::Mouse::Button::Left) && itemUnderMouse >= 0) {
        if (itemUnderMouse == count) {
            state = GameState::Menu;
            MenuScreen::setViewportOffset(0);
            return;
        }
        const int legendIndex = count - 1;
        if (itemUnderMouse == legendIndex && !legendUnlocked) {
            if (walletCoins >= LEGEND_PRICE) {
                walletCoins -= LEGEND_PRICE;
                legendUnlocked = true;
            }
            return;
        }
        if (!profileUnlocked(itemUnderMouse)) return;
        if (profileLevels[static_cast<std::size_t>(itemUnderMouse)] >= 2) return;
        if (walletCoins < UPGRADE_PRICE) return;
        walletCoins -= UPGRADE_PRICE;
        ++profileLevels[static_cast<std::size_t>(itemUnderMouse)];
    }
}

Player::Profile Game::upgradedProfile(int profileIndex) const {
    Player::Profile profile = Player::profiles()[static_cast<std::size_t>(profileIndex)];
    if (profileIndex >= 0 && profileIndex < static_cast<int>(profileLevels.size())) {
        profile.maxHealth += profileLevels[static_cast<std::size_t>(profileIndex)];
    }
    return profile;
}

bool Game::profileUnlocked(int profileIndex) const {
    const int legendIndex = static_cast<int>(Player::profiles().size()) - 1;
    return profileIndex != legendIndex || legendUnlocked;
}

void Game::moveProfileSelection(int player, int delta) {
    const int count = static_cast<int>(Player::profiles().size());
    int next = selectedProfiles[player];
    for (int attempts = 0; attempts < count; ++attempts) {
        next = (next + delta + count) % count;
        if (profileUnlocked(next)) {
            selectedProfiles[player] = next;
            return;
        }
    }
}

void Game::updateCamera(float dt) {
    const auto& players = level.getPlayers();
    sf::Vector2f target;
    int activeCount = 0;
    for (const auto& player : players) {
        if (player->isRespawning()) continue;
        target += player->position + player->size * 0.5f;
        ++activeCount;
    }
    if (activeCount == 0) {
        for (const auto& player : players) target += player->position + player->size * 0.5f;
        activeCount = static_cast<int>(players.size());
    }
    if (activeCount > 0) target /= static_cast<float>(activeCount);

    camera = Camera::follow(
        camera,
        target,
        window.getView().getSize(),
        {static_cast<float>(level.getTileMap().widthPixels()), static_cast<float>(level.getTileMap().heightPixels())},
        dt
    );
}

void Game::render() {
    window.clear({16, 19, 24});

    sf::Sprite background(assets.texture("background"));
    background.setPosition({0.0f, 0.0f});
    window.draw(background);

    const bool showLevel = state == GameState::Playing || state == GameState::Paused ||
                           state == GameState::Controls || state == GameState::Victory || state == GameState::GameOver;
    if (showLevel) {
        level.draw(window, camera);
        hud.draw(window, level);
    }

    // Draw achievement notification
    if (achievementDisplayTimer > 0.0f) {
        HUD::drawAchievement(window, currentAchievementText, {20.0f, static_cast<float>(Constants::WINDOW_HEIGHT) - 80.0f}, achievementDisplayTimer);
    }
    menu.draw(window, state, menuIndex, selectedProfiles, activeSelectPlayer, playerCount, audio.getMasterVolume(), gameSpeed,
              selectedLevel, unlockedLevelCount, selectedDifficulty, walletCoins, shopIndex, legendUnlocked, profileLevels,
              pauseMenuIndex, controlsPlayerIndex, controlsActionIndex, rebinding, rebindWarning, input);
    window.display();
}

void Game::saveGame() {
    try {
        saveData.walletCoins = walletCoins;
        saveData.unlockedLevelCount = unlockedLevelCount;
        saveData.legendUnlocked = legendUnlocked;
        saveData.profileLevels = profileLevels;

        // Convert bool achievements to char for serialization
        const auto& boolAchievements = level.getAchievements();
        saveData.achievements.resize(boolAchievements.size());
        for (size_t i = 0; i < boolAchievements.size(); ++i) {
            saveData.achievements[i] = boolAchievements[i] ? 1 : 0;
        }

        saveData.saveToFile();
    } catch (const std::exception& e) {
        // Silently fail if saving doesn't work
    }
}

void Game::loadGame() {
    try {
        if (saveData.loadFromFile()) {
            walletCoins = saveData.walletCoins;
            unlockedLevelCount = saveData.unlockedLevelCount;
            legendUnlocked = saveData.legendUnlocked;
            profileLevels = saveData.profileLevels;

            // Convert char achievements back to bool
            std::vector<bool> boolAchievements;
            if (saveData.achievements.size() > 0) {
                boolAchievements.resize(saveData.achievements.size());
                for (size_t i = 0; i < saveData.achievements.size(); ++i) {
                    boolAchievements[i] = saveData.achievements[i] != 0;
                }
            }
            level.setAchievements(boolAchievements);
        } else {
            // First time playing - unlock all levels for testing
            unlockedLevelCount = 4; // Unlock all 4 levels
        }
    } catch (const std::exception& e) {
        // If loading fails, reset to defaults
        walletCoins = 0;
        unlockedLevelCount = 4;
        legendUnlocked = false;
        profileLevels.assign(Player::profiles().size(), 0);
        std::vector<bool> emptyAchievements(4, false);
        level.setAchievements(emptyAchievements);
    }
}

void Game::resetGame() {
    saveData.reset();
    saveData.saveToFile();
    walletCoins = 0;
    unlockedLevelCount = 1;
    legendUnlocked = false;
    profileLevels.assign(Player::profiles().size(), 0);
    level.resetAchievements();
}
