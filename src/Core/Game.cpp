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
    EnemyTextures::setTextures(assets.texture("patrol"),
                               assets.texture("mushroomIdle"), assets.texture("mushroomAttack"),
                               assets.texture("mushroomDie"),
                               assets.texture("flyingIdle"), assets.texture("flyingHurt"), assets.texture("flyingDie"),
                               assets.texture("bossIdle"), assets.texture("bossWalk"), assets.texture("bossHurt"),
                               assets.texture("bossDie"), assets.texture("bossAttack"), assets.texture("bossProjectile"));
    Projectile::setTextures(assets.texture("mushroomProjectile"), assets.texture("bossProjectile"));
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
    assets.LoadTexture("rush", "assets/textures/rush.png");
    if (!assets.LoadTexture("background", "assets/textures/background.png") ||
        !assets.LoadTexture("selector", "assets/textures/selector.png") ||
        !assets.LoadTexture("solid", "assets/textures/solid.png") ||
        !assets.LoadTexture("goal", "assets/textures/goal.png") ||
        !assets.LoadTexture("spike", "assets/textures/spike.png") ||
        !assets.LoadTexture("ori", "assets/textures/ori.png") ||
        (!assets.LoadTexture("patrol", "assets/textures/slime.png") && !assets.LoadTexture("patrol", "assets/textures/patrol.png")) ||
        !assets.LoadTexture("mushroomIdle", "assets/textures/mushroom/mushroom_idle.png") ||
        !assets.LoadTexture("mushroomAttack", "assets/textures/mushroom/mushroom_attack.png") ||
        !assets.LoadTexture("mushroomDie", "assets/textures/mushroom/mushroom_die.png") ||
        !assets.LoadTexture("bossIdle", "assets/textures/boss/Golem_idle.png") ||
        !assets.LoadTexture("bossWalk", "assets/textures/boss/Golem_walk.png") ||
        !assets.LoadTexture("bossHurt", "assets/textures/boss/Golem_hurt.png") ||
        !assets.LoadTexture("bossDie", "assets/textures/boss/Golem_die.png") ||
        !assets.LoadTexture("bossAttack", "assets/textures/boss/Golem_attack.png") ||
        !assets.LoadTexture("bossProjectile", "assets/textures/boss/Golem_projectile.png") ||
        !assets.LoadTexture("mushroomProjectile", "assets/textures/mushroom/mushroom_projectile.png") ||
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
    input.update();
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButton->button == sf::Mouse::Button::Left) {
                    handleMouseClick(window.mapPixelToCoords(mouseButton->position));
                }
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (state == GameState::Controls) settings.handleKeyPressed(keyPressed->code, input);
            }
        }

        input.update();
        updateUiHover();
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
    if (input.isActionPressed(Action::Pause) &&
        (state == GameState::Playing || state == GameState::Paused)) {
        state = state == GameState::Playing ? GameState::Paused : GameState::Playing;
    }
    if (state == GameState::Menu || state == GameState::Info || state == GameState::LevelSelect ||
        state == GameState::DifficultySelect || state == GameState::Shop || state == GameState::CharacterSelect ||
        state == GameState::Controls || state == GameState::Paused || state == GameState::Victory ||
        state == GameState::GameOver) return;
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

void Game::updateUiHover() {
    const sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    if (state == GameState::Controls) {
        settings.updateHover(mousePosition);
        return;
    }
    menu.updateHover(state, mousePosition, static_cast<int>(Player::profiles().size()));
}

void Game::handleMouseClick(sf::Vector2f mousePosition) {
    if (state == GameState::Playing) return;
    if (state == GameState::Controls) {
        if (settings.handleMouseClick(mousePosition)) state = settingsReturnState;
        return;
    }

    const int buttonIndex = menu.buttonAt(state, mousePosition, static_cast<int>(Player::profiles().size()));
    if (buttonIndex >= 0) handleMenuClick(buttonIndex);
}

void Game::handleMenuClick(int buttonIndex) {
    if (state == GameState::Menu) {
        if (buttonIndex == 0 || buttonIndex == 1) {
            playerCount = buttonIndex + 1;
            activeSelectPlayer = 0;
            state = GameState::LevelSelect;
        } else if (buttonIndex == 2) {
            state = GameState::DifficultySelect;
        } else if (buttonIndex == 3) {
            state = GameState::Shop;
        } else if (buttonIndex == 4) {
            state = GameState::Info;
        } else if (buttonIndex == 5) {
            openSettings(GameState::Menu);
        } else if (buttonIndex == 6) {
            window.close();
        }
        return;
    }

    if (state == GameState::Info) {
        state = GameState::Menu;
        return;
    }

    if (state == GameState::LevelSelect) {
        if (buttonIndex == static_cast<int>(LEVELS.size())) {
            state = GameState::Menu;
        } else if (buttonIndex < unlockedLevelCount) {
            selectedLevel = buttonIndex;
            activeSelectPlayer = 0;
            state = GameState::CharacterSelect;
        }
        return;
    }

    if (state == GameState::DifficultySelect) {
        if (buttonIndex == static_cast<int>(DIFFICULTIES.size())) {
            state = GameState::Menu;
        } else {
            selectedDifficulty = buttonIndex;
            state = GameState::Menu;
        }
        return;
    }

    if (state == GameState::CharacterSelect) {
        const int profileCount = static_cast<int>(Player::profiles().size());
        if (buttonIndex < profileCount) {
            if (!profileUnlocked(buttonIndex)) return;
            selectedProfiles[activeSelectPlayer] = buttonIndex;
            if (playerCount == 2) activeSelectPlayer = 1 - activeSelectPlayer;
        } else if (buttonIndex == profileCount) {
            restart();
        } else {
            activeSelectPlayer = 0;
            state = GameState::LevelSelect;
        }
        return;
    }

    if (state == GameState::Shop) {
        handleShopClick(buttonIndex);
        return;
    }

    if (state == GameState::Paused) {
        if (buttonIndex == 0) state = GameState::Playing;
        if (buttonIndex == 1) openSettings(GameState::Paused);
        if (buttonIndex == 2) state = GameState::Menu;
        return;
    }

    if (state == GameState::Victory || state == GameState::GameOver) {
        if (buttonIndex == 0) restart();
        if (buttonIndex == 1) state = GameState::Menu;
    }
}

void Game::handleShopClick(int buttonIndex) {
    const int count = static_cast<int>(Player::profiles().size());
    shopIndex = buttonIndex;
    if (buttonIndex == count) {
        state = GameState::Menu;
        return;
    }
    if (buttonIndex < 0 || buttonIndex >= count) return;

    const int legendIndex = count - 1;
    if (buttonIndex == legendIndex && !legendUnlocked) {
        if (walletCoins >= LEGEND_PRICE) {
            walletCoins -= LEGEND_PRICE;
            legendUnlocked = true;
        }
        return;
    }
    if (!profileUnlocked(buttonIndex)) return;
    if (profileLevels[static_cast<std::size_t>(buttonIndex)] >= 2) return;
    if (walletCoins < UPGRADE_PRICE) return;
    walletCoins -= UPGRADE_PRICE;
    ++profileLevels[static_cast<std::size_t>(buttonIndex)];
}

void Game::openSettings(GameState returnState) {
    settingsReturnState = returnState;
    state = GameState::Controls;
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
    if (state == GameState::Controls) settings.draw(window, input);
    window.display();
}
