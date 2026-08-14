#pragma once

#include "Graphics/Animation.h"

#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <unordered_map>

struct CharacterSpriteSet {
    const sf::Texture* texture = nullptr;
    int frameWidth = 0;
    int frameHeight = 0;
    Animation run;
    Animation idle;
    Animation jump;
    float runSpeedThreshold = 5.0f;

    sf::IntRect frameRect(int frameIndex) const;
};

class CharacterSprites {
public:
    static void registerSet(const std::string& id, CharacterSpriteSet set);
    static const CharacterSpriteSet* get(const std::string& id);

private:
    static std::unordered_map<std::string, CharacterSpriteSet>& registry();
};
