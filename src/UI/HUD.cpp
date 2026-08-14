#include "UI/HUD.h"

#include "Entities/Inventory.h"
#include "Entities/Player.h"
#include "Utils/Constants.h"
#include "World/Level.h"

#include <algorithm>
#include <cmath>
#include <string>

namespace {
const sf::Font* uiFont = nullptr;
const sf::Texture* heartTexPtr = nullptr;
const sf::Texture* meatTexPtr = nullptr;
const sf::Texture* foodTexPtr = nullptr;
const sf::Texture* coinTexPtr = nullptr;
const sf::Texture* solidTexPtr = nullptr;
sf::Texture fallbackHeartTex;
sf::Texture fallbackMeatTex;

const sf::Texture& getHeartTexture() {
    if (heartTexPtr) return *heartTexPtr;
    if (fallbackHeartTex.getNativeHandle() == 0) {
        (void)fallbackHeartTex.loadFromFile("assets/textures/heart.png");
    }
    return fallbackHeartTex;
}

const sf::Texture& getMeatTexture() {
    if (meatTexPtr) return *meatTexPtr;
    if (fallbackMeatTex.getNativeHandle() == 0) {
        (void)fallbackMeatTex.loadFromFile("assets/textures/meat.png");
    }
    return fallbackMeatTex;
}
}

void HUD::setFont(const sf::Font& font) {
    uiFont = &font;
}

void HUD::setTextures(const sf::Texture& heart, const sf::Texture& meat,
                      const sf::Texture& food, const sf::Texture& coin, const sf::Texture& solid) {
    heartTexPtr = &heart;
    meatTexPtr = &meat;
    foodTexPtr = &food;
    coinTexPtr = &coin;
    solidTexPtr = &solid;
}

void HUD::drawText(sf::RenderWindow& window, const std::string& text, sf::Vector2f position, unsigned size, sf::Color color) {
    if (!uiFont) return;
    sf::Text label(*uiFont, text, size);
    label.setPosition(position);
    label.setFillColor(color);
    window.draw(label);
}

void HUD::drawTextCentered(sf::RenderWindow& window, const std::string& text, sf::Vector2f center, unsigned size, sf::Color color) {
    if (!uiFont) return;
    sf::Text label(*uiFont, text, size);
    const sf::FloatRect bounds = label.getLocalBounds();
    label.setOrigin({bounds.size.x * 0.5f, bounds.size.y * 0.5f});
    label.setPosition(center);
    label.setFillColor(color);
    window.draw(label);
}

void HUD::drawHearts(sf::RenderWindow& window, sf::Vector2f position, float health, int maxHealth) {
    const sf::Texture& tex = getHeartTexture();
    sf::Sprite heartSprite(tex);
    const float frameSize = 32.0f;
    const float renderSize = 22.0f;
    const float scale = renderSize / frameSize;
    heartSprite.setScale({scale, scale});

    for (int i = 0; i < maxHealth; ++i) {
        float remaining = std::clamp(health - static_cast<float>(i), 0.0f, 1.0f);
        int col = 4;
        if (remaining >= 0.875f) col = 0;
        else if (remaining >= 0.625f) col = 1;
        else if (remaining >= 0.375f) col = 2;
        else if (remaining >= 0.125f) col = 3;

        heartSprite.setTextureRect(sf::IntRect({col * 32, 0}, {32, 32}));
        heartSprite.setPosition({position.x + static_cast<float>(i) * (renderSize + 3.0f), position.y});
        window.draw(heartSprite);
    }
}

void HUD::drawHungerBar(sf::RenderWindow& window, sf::Vector2f position, float hunger, float maxHunger) {
    const sf::Texture& tex = getMeatTexture();
    sf::Sprite meatSprite(tex);
    const float iconSize = 18.0f;
    const float scale = iconSize / 16.0f;
    meatSprite.setScale({scale, scale});
    meatSprite.setPosition(position);
    window.draw(meatSprite);

    const float barWidth = 140.0f;
    const float barHeight = 12.0f;
    const sf::Vector2f barPos = {position.x + iconSize + 6.0f, position.y + 3.0f};

    sf::RectangleShape bg({barWidth, barHeight});
    bg.setPosition(barPos);
    bg.setFillColor(sf::Color(25, 28, 36, 220));
    bg.setOutlineThickness(1.0f);
    bg.setOutlineColor(sf::Color(65, 72, 85));
    window.draw(bg);

    float ratio = std::clamp(hunger / std::max(1.0f, maxHunger), 0.0f, 1.0f);
    if (ratio > 0.0f) {
        sf::RectangleShape fill({barWidth * ratio, barHeight});
        fill.setPosition(barPos);
        if (ratio > 0.5f) fill.setFillColor(sf::Color(245, 158, 11));
        else if (ratio > 0.25f) fill.setFillColor(sf::Color(217, 119, 6));
        else fill.setFillColor(sf::Color(220, 38, 38));
        window.draw(fill);
    }
}

namespace {
void drawInventorySlot(sf::RenderWindow& window, sf::Vector2f position, int slotIndex, int count,
                       bool selected, float actionTimer) {
    constexpr float slotSize = 24.0f;
    constexpr float iconSize = static_cast<float>(Constants::ITEM_ICON_SIZE);
    constexpr float iconScale = 18.0f / iconSize;

    sf::RectangleShape slot({slotSize, slotSize});
    slot.setPosition(position);
    slot.setFillColor(sf::Color(25, 28, 36, 220));
    slot.setOutlineThickness(selected ? 2.0f : 1.0f);
    slot.setOutlineColor(selected ? sf::Color(246, 233, 190) : sf::Color(65, 72, 85));
    window.draw(slot);

    if (actionTimer > 0.0f && selected) {
        sf::RectangleShape cooldown({slotSize, slotSize * (1.0f - std::clamp(actionTimer / Constants::EAT_ACTION_TIME, 0.0f, 1.0f))});
        cooldown.setPosition({position.x, position.y + slotSize * std::clamp(actionTimer / Constants::EAT_ACTION_TIME, 0.0f, 1.0f)});
        cooldown.setFillColor(sf::Color(255, 255, 255, 60));
        window.draw(cooldown);
    }

    const sf::Vector2f iconPos = position + sf::Vector2f((slotSize - iconSize * iconScale) * 0.5f, (slotSize - iconSize * iconScale) * 0.5f);

    if (slotIndex < FOOD_TYPE_COUNT && foodTexPtr) {
        sf::Sprite icon(*foodTexPtr);
        icon.setTextureRect(sf::IntRect({slotIndex * Constants::ITEM_ICON_SIZE, 0}, {Constants::ITEM_ICON_SIZE, Constants::ITEM_ICON_SIZE}));
        icon.setScale({iconScale, iconScale});
        icon.setPosition(iconPos);
        window.draw(icon);
    } else if (slotIndex == COIN_SLOT_INDEX && coinTexPtr) {
        sf::Sprite icon(*coinTexPtr);
        const int frame = count > 0 ? std::min(count - 1, Constants::COIN_FRAME_COUNT - 1) : 0;
        icon.setTextureRect(sf::IntRect({frame * Constants::ITEM_ICON_SIZE, 0}, {Constants::ITEM_ICON_SIZE, Constants::ITEM_ICON_SIZE}));
        icon.setScale({iconScale, iconScale});
        icon.setPosition(iconPos);
        window.draw(icon);
    } else if (slotIndex == BLOCK_SLOT_INDEX && solidTexPtr) {
        sf::Sprite icon(*solidTexPtr);
        icon.setScale({18.0f / 32.0f, 18.0f / 32.0f});
        icon.setPosition(iconPos);
        window.draw(icon);
    }

    if (count > 0) {
        HUD::drawText(window, std::to_string(count), {position.x + 13.0f, position.y + 11.0f}, 11, sf::Color::White);
    }
}
}

void HUD::drawPanel(sf::RenderWindow& window, const Player& player, sf::Vector2f position, const std::string& label) {
    const float cardWidth = 220.0f;
    const float cardHeight = 92.0f;
    sf::RectangleShape card({cardWidth, cardHeight});
    card.setPosition(position);
    card.setFillColor(sf::Color(16, 20, 26, 210));
    card.setOutlineThickness(1.5f);
    card.setOutlineColor(player.getProfile().color);
    window.draw(card);

    drawText(window, label + " - " + player.getProfile().name, {position.x + 8.0f, position.y + 4.0f}, 13, player.getProfile().color);

    if (player.isRespawning()) {
        drawText(window, "RESPAWNING...", {position.x + 8.0f, position.y + 26.0f}, 15, sf::Color(239, 68, 68));
    } else {
        drawHearts(window, {position.x + 8.0f, position.y + 22.0f}, static_cast<float>(player.getHealth()), player.getMaxHealth());
        drawHungerBar(window, {position.x + 8.0f, position.y + 44.0f}, player.getHunger(), static_cast<float>(Constants::PLAYER_MAX_HUNGER));

        const auto& inventory = player.getInventory();
        for (int i = 0; i < INVENTORY_SLOTS; ++i) {
            drawInventorySlot(window, {position.x + 8.0f + static_cast<float>(i) * 28.0f, position.y + 64.0f},
                              i, inventory.getSlot(i), i == inventory.selectedSlot, inventory.actionTimer);
        }
    }
}

void HUD::drawBar(sf::RenderWindow& window, sf::Vector2f position, float width, float height, float ratio, sf::Color color) {
    sf::RectangleShape bg({width, height});
    bg.setPosition(position);
    bg.setFillColor(sf::Color(40, 44, 52));
    window.draw(bg);

    if (ratio > 0.0f) {
        sf::RectangleShape fill({width * std::clamp(ratio, 0.0f, 1.0f), height});
        fill.setPosition(position);
        fill.setFillColor(color);
        window.draw(fill);
    }
}

void HUD::draw(sf::RenderWindow& window, const Level& level) const {
    const auto& players = level.getPlayers();
    for (std::size_t i = 0; i < players.size(); ++i) {
        sf::Vector2f pos = {20.0f, 20.0f + static_cast<float>(i) * 102.0f};
        std::string label = "P" + std::to_string(i + 1);
        drawPanel(window, *players[i], pos, label);
    }
}
