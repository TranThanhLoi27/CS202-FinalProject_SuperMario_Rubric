#include "Graphics/CharacterSprites.h"

sf::IntRect CharacterSpriteSet::frameRect(int frameIndex) const {
    return sf::IntRect(
        {frameIndex * frameWidth, 0},
        {frameWidth, frameHeight}
    );
}

std::unordered_map<std::string, CharacterSpriteSet>& CharacterSprites::registry() {
    static std::unordered_map<std::string, CharacterSpriteSet> sets;
    return sets;
}

void CharacterSprites::registerSet(const std::string& id, CharacterSpriteSet set) {
    registry()[id] = std::move(set);
}

const CharacterSpriteSet* CharacterSprites::get(const std::string& id) {
    if (id.empty()) return nullptr;
    const auto it = registry().find(id);
    return it == registry().end() ? nullptr : &it->second;
}
