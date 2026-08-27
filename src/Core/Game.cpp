// Runs the application loop and coordinates state transitions between modules.
#include "Core/Game.h"

#include "Core/Camera.h"
#include "Entities/DroppedItem.h"
#include "Entities/Inventory.h"
#include "Entities/Projectile.h"
#include "Entities/Tombstone.h"
#include "Graphics/CharacterSprites.h"
#include "Graphics/EnemySprites.h"
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

constexpr int LEGEND_PRICE = 50;
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
    MenuScreen::setSelectorTexture(assets.texture("selector"));
    Level::setTextures(assets.texture("solid"), assets.texture("goal"), assets.texture("spike"));
    HUD::setTextures(assets.texture("heart"), assets.texture("meat"), assets.texture("food"),
                      assets.texture("coin"), assets.texture("solid"));
    EnemyTextures::setTextures(assets.texture("patrol"), assets.texture("shooter"), assets.texture("flying"),
                               assets.texture("boss"), assets.texture("boss_projectile"));
    Projectile::setTexture(assets.texture("boss_projectile"));
    DroppedItem::setTextures(assets.texture("food"), assets.texture("coin"), assets.texture("solid"));
    registerCharacterSprites();
    Tombstone::setTexture(assets.texture("tombstone"));

    profileLevels.assign(Player::profiles().size(), 0);
    level.loadDefault();
    audio.setMasterVolume(70.0f);
    audio.playMusic("assets/audio/background.ogg");
}

void Game::loadTexture(){
    assets.LoadTexture("heart", "assets/textures/heart.png");
    assets.LoadTexture("meat", "assets/textures/meat.png");
    assets.LoadTexture("food", "assets/textures/food.png");
    assets.LoadTexture("coin", "assets/textures/coin.png");
    if (!assets.LoadTexture("background", "assets/textures/background.png") ||
        !assets.LoadTexture("selector", "assets/textures/selector.png") ||
        !assets.LoadTexture("solid", "assets/textures/solid.png") ||
        !assets.LoadTexture("goal", "assets/textures/goal.png") ||
        !assets.LoadTexture("spike", "assets/textures/spike.png") ||
        !assets.LoadTexture("ori", "assets/textures/ori.png") ||
        (!assets.LoadTexture("patrol", "assets/textures/slime.png") && !assets.LoadTexture("patrol", "assets/textures/patrol.png")) ||
        !assets.LoadTexture("shooter", "assets/textures/shooter.png") ||
        !assets.LoadTexture("boss", "assets/textures/boss.png") ||
        !assets.LoadTexture("boss_projectile", "assets/textures/boss_projectile.png") ||
        !assets.LoadTexture("tombstone", "assets/textures/tombstone.png")) {
        throw std::runtime_error("Could not load required textures");
    }
    if (!assets.LoadTexture("flying", "assets/textures/flying.png")) {
        assets.LoadTexture("flying", "assets/textures/shooter.png");
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
}

void Game::run() {
    sf::Clock clock;
    input.update();
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        input.update();
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
    if (input.pressed(sf::Keyboard::Key::Enter) &&
        (state == GameState::Victory || state == GameState::GameOver)) {
        restart();
    }
    if (input.pressed(sf::Keyboard::Key::Escape) &&
        (state == GameState::Victory || state == GameState::GameOver)) {
        state = GameState::Menu;
    }
    if (input.pressed(sf::Keyboard::Key::P) &&
        (state == GameState::Playing || state == GameState::Paused)) {
        state = state == GameState::Playing ? GameState::Paused : GameState::Playing;
    }
    if (state == GameState::Paused) {
        updatePaused();
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
    if (state == GameState::Info) {
        if (input.pressed(sf::Keyboard::Key::Enter) || input.pressed(sf::Keyboard::Key::Escape)) {
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

    if (input.pressed(sf::Keyboard::Key::Up)) menuIndex = (menuIndex + 5) % 6;
    if (input.pressed(sf::Keyboard::Key::Down)) menuIndex = (menuIndex + 1) % 6;
    if (input.pressed(sf::Keyboard::Key::I)) {
        state = GameState::Info;
        return;
    }
    if (!input.pressed(sf::Keyboard::Key::Enter)) return;

    if (menuIndex == 0 || menuIndex == 1) {
        playerCount = menuIndex + 1;
        activeSelectPlayer = 0;
        state = GameState::LevelSelect;
    }
    if (menuIndex == 2) state = GameState::DifficultySelect;
    if (menuIndex == 3) state = GameState::Shop;
    if (menuIndex == 4) state = GameState::Info;
    if (menuIndex == 5) window.close();
}

void Game::updatePaused() {
    if (input.pressed(sf::Keyboard::Key::Escape)) {
        state = GameState::Menu;
        return;
    }
    if (input.pressed(sf::Keyboard::Key::Left)) gameSpeed = std::max(0.25f, gameSpeed - 0.25f);
    if (input.pressed(sf::Keyboard::Key::Right)) gameSpeed = std::min(2.0f, gameSpeed + 0.25f);
    if (input.pressed(sf::Keyboard::Key::Down)) audio.setMasterVolume(audio.getMasterVolume() - 10.0f);
    if (input.pressed(sf::Keyboard::Key::Up)) audio.setMasterVolume(audio.getMasterVolume() + 10.0f);
}

void Game::updateMapSelect() {
    if (input.pressed(sf::Keyboard::Key::Escape)) {
        state = GameState::Menu;
        return;
    }
    if (input.pressed(sf::Keyboard::Key::Up)) selectedLevel = (selectedLevel + static_cast<int>(LEVELS.size()) - 1) % static_cast<int>(LEVELS.size());
    if (input.pressed(sf::Keyboard::Key::Down)) selectedLevel = (selectedLevel + 1) % static_cast<int>(LEVELS.size());
    if (selectedLevel >= unlockedLevelCount) selectedLevel = std::max(0, unlockedLevelCount - 1);
    if (input.pressed(sf::Keyboard::Key::Enter) && selectedLevel < unlockedLevelCount) {
        activeSelectPlayer = 0;
        state = GameState::CharacterSelect;
    }
}

void Game::updateDifficultySelect() {
    if (input.pressed(sf::Keyboard::Key::Escape)) {
        state = GameState::Menu;
        return;
    }
    if (input.pressed(sf::Keyboard::Key::Up)) selectedDifficulty = (selectedDifficulty + 2) % 3;
    if (input.pressed(sf::Keyboard::Key::Down)) selectedDifficulty = (selectedDifficulty + 1) % 3;
    if (input.pressed(sf::Keyboard::Key::Enter)) {
        state = GameState::Menu;
    }
}

void Game::updateCharacterSelect() {
    if (input.pressed(sf::Keyboard::Key::A)) {
        moveProfileSelection(0, -1);
        activeSelectPlayer = 0;
    }
    if (input.pressed(sf::Keyboard::Key::D)) {
        moveProfileSelection(0, 1);
        activeSelectPlayer = 0;
    }
    if (playerCount >= 2 && input.pressed(sf::Keyboard::Key::Left)) {
        moveProfileSelection(1, -1);
        activeSelectPlayer = 1;
    }
    if (playerCount >= 2 && input.pressed(sf::Keyboard::Key::Right)) {
        moveProfileSelection(1, 1);
        activeSelectPlayer = 1;
    }
    if (input.pressed(sf::Keyboard::Key::Escape)) state = GameState::LevelSelect;
    if (input.pressed(sf::Keyboard::Key::Enter)) restart();
}

void Game::updateShop() {
    const int count = static_cast<int>(Player::profiles().size());
    const int rows = count + 1;
    if (input.pressed(sf::Keyboard::Key::Escape)) {
        state = GameState::Menu;
        return;
    }
    if (input.pressed(sf::Keyboard::Key::Up)) shopIndex = (shopIndex + rows - 1) % rows;
    if (input.pressed(sf::Keyboard::Key::Down)) shopIndex = (shopIndex + 1) % rows;
    if (!input.pressed(sf::Keyboard::Key::Enter)) return;
    if (shopIndex == count) {
        state = GameState::Menu;
        return;
    }
    const int legendIndex = count - 1;
    if (shopIndex == legendIndex && !legendUnlocked) {
        if (walletCoins >= LEGEND_PRICE) {
            walletCoins -= LEGEND_PRICE;
            legendUnlocked = true;
        }
        return;
    }
    if (!profileUnlocked(shopIndex)) return;
    if (profileLevels[static_cast<std::size_t>(shopIndex)] >= 2) return;
    if (walletCoins < UPGRADE_PRICE) return;
    walletCoins -= UPGRADE_PRICE;
    ++profileLevels[static_cast<std::size_t>(shopIndex)];
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
                           state == GameState::Victory || state == GameState::GameOver;
    if (showLevel) {
        level.draw(window, camera);
        hud.draw(window, level);
    }
    menu.draw(window, state, menuIndex, selectedProfiles, activeSelectPlayer, playerCount, audio.getMasterVolume(), gameSpeed,
              selectedLevel, unlockedLevelCount, selectedDifficulty, walletCoins, shopIndex, legendUnlocked, profileLevels);
    window.display();
}
