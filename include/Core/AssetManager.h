#pragma once

#include<SFML/Graphics.hpp>

#include <map>
#include <string>

class AssetManager{
public:
    //load texture 
    bool LoadTexture(const std::string& id, const std::string& path);
    //load font
    bool LoadFont(const std::string& id, const std::string& path);
    //return pointer texture
    const sf::Texture* texture(const std::string& id) const;
    //return pointer font
    const sf::Font* font(const std::string& id) const;
    //clear textures & fonts
    void Clear();
private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
};