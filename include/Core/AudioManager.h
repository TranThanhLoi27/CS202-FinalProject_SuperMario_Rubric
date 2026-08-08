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
    //play all buffers
    void PlaySound(const std::string& id);
    //play music
    void PlayMusic(const std::string& path, bool loop = true);
    //change volume
    void SetVolume(const float& volume);
    //remote buffer was stopped
    void Update();
    //remote all buffers & sounds
    void Clear();
private:
    std::map<std::string, sf::SoundBuffer> buffers;
    std::list<sf::Sound> sounds;
    sf::Music music;
    float mastervolume = 70.f;
};