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
    Animation attack;
    float runSpeedThreshold = 5.0f;

    /// Returns a texture rectangle for a frame in this sprite grid.
    sf::IntRect frameRect(int frameIndex) const;
};

/// Registry that exposes shared sprite metadata by logical identifier.
class CharacterSprites {
public:
    /// Registers or replaces a shared character sprite set by identifier.
    static void registerSet(const std::string& id, CharacterSpriteSet set);
    /// Finds a registered sprite set or returns null.
    static const CharacterSpriteSet* get(const std::string& id);

private:
    /// Returns the process-local Registry-pattern storage.
    static std::unordered_map<std::string, CharacterSpriteSet>& registry();
};
