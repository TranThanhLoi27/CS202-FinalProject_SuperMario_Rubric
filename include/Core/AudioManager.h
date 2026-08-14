// Loads and plays optional sound effects and background music.
#pragma once

#include <SFML/Audio.hpp>
#include <list>
#include <map>
#include <string>

class AudioManager {
public:
    bool loadSound(const std::string& id, const std::string& path);
    bool playMusic(const std::string& path, bool loop = true);
    void play(const std::string& id);
    void setMasterVolume(float volume);
    float getMasterVolume() const;
    void update();
    void clear();

private:
    std::map<std::string, sf::SoundBuffer> buffers;
    std::list<sf::Sound> sounds;
    sf::Music music;
    float masterVolume = 70.0f;
};
