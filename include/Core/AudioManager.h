// Loads and plays optional sound effects and background music.
#pragma once

#include <SFML/Audio.hpp>
#include <list>
#include <map>
#include <string>
#include <utility>

class AudioManager {
public:
    bool loadSound(const std::string& id, const std::string& path);
    bool playMusic(const std::string& path, bool loop = true);
    void play(const std::string& id);
    void stop(const std::string& id);
    void stopMusic();
    void setMasterVolume(float volume);
    float getMasterVolume() const;
    void update();
    void clear();

private:
    struct ActiveSound {
        ActiveSound(std::string soundId, const sf::SoundBuffer& buffer)
            : id(std::move(soundId)), sound(buffer) {}

        std::string id;
        sf::Sound sound;
    };

    std::map<std::string, sf::SoundBuffer> buffers;
    std::list<ActiveSound> sounds;
    sf::Music music;
    float masterVolume = 70.0f;
};
