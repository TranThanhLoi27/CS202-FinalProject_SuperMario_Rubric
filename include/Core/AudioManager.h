#pragma once

#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

#include<list>
#include<map>
#include<string>
class AudioManager{
public:
    //load buffers
    bool LoadSound(const std::string& id, const std::string& path);
    void PlaySound(const std::string& id);
    void PlayMusic(const std::string& path, bool loop = true);
    void SetVolume(const float& volume);
    void Update();
    void Clear();
private:
    std::map<std::string, sf::SoundBuffer> buffers;
    std::list<sf::Sound> sounds;
    sf::Music music;
    float mastervolume = 70.f;
};