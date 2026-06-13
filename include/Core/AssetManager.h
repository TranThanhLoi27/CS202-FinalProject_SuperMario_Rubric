#pragma once

#include<SFML/Graphics.hpp>

#include <map>
#include <string>

class AssetManager{
public:
    //nap texture 
    bool LoadTexture(const std::string& id, const std::string& path);
    //nap font
    bool LoadFont(const std::string& id, const std::string& path);
    //lay texture
    const sf::Texture* texture(const std::string& id) const;
    //lay font
    const sf::Font* font(const std::string& id) const;
    //clear textures & fonts
    void clear();
private:
    std::map<std::string, sf::Texture> textures;
    std::map<std::string, sf::Font> fonts;
};