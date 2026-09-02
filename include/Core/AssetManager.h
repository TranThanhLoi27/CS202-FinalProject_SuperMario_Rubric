#pragma once

#include<SFML/Graphics.hpp>

#include <map>
#include <string>

/// Flyweight-style resource cache that shares one texture or font per identifier.
class AssetManager{
public:
    /// Loads a texture from disk and stores it under a logical identifier.
    bool LoadTexture(const std::string& id, const std::string& path);
    /// Loads a font from disk and stores it under a logical identifier.
    bool LoadFont(const std::string& id, const std::string& path);
    /// Returns the shared texture registered under the requested identifier.
    const sf::Texture& texture(const std::string& id) const;
    /// Returns the shared font registered under the requested identifier.
    const sf::Font& font(const std::string& id) const;
    /// Releases every cached texture and font.
    void Clear();
private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
};
