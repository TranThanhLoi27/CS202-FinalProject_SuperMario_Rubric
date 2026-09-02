#include "Core/AudioManager.h"

#include <algorithm>
#include <iostream>
#include <utility>

bool AudioManager::loadSound(const std::string& id, const std::string& path) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(path)) {
        std::cerr << "Audio warning: could not load sound '" << id
                  << "' from " << path << '\n';
        return false;
    }

    stop(id);
    buffers[id] = std::move(buffer);
    return true;
}

void AudioManager::play(const std::string& id) {
    const auto it = buffers.find(id);
    if (it == buffers.end()) return;

    sounds.emplace_back(id, it->second);
    sounds.back().sound.setVolume(masterVolume);
    sounds.back().sound.play();
}

void AudioManager::stop(const std::string& id) {
    for (auto it = sounds.begin(); it != sounds.end();) {
        if (it->id != id) {
            ++it;
            continue;
        }
        it->sound.stop();
        it = sounds.erase(it);
    }
}

bool AudioManager::playMusic(const std::string& path, bool loop) {
    music.stop();
    if (!music.openFromFile(path)) {
        std::cerr << "Audio warning: could not load music from " << path << '\n';
        return false;
    }
    music.setLooping(loop);
    music.setVolume(masterVolume);
    music.play();
    return true;
}

void AudioManager::stopMusic() {
    music.stop();
}

void AudioManager::update() {
    for (auto it = sounds.begin(); it != sounds.end();) {
        if (it->sound.getStatus() == sf::Sound::Status::Stopped) {
            it = sounds.erase(it);
        } else {
            ++it;
        }
    }
}

void AudioManager::setMasterVolume(float volume) {
    masterVolume = std::clamp(volume, 0.0f, 100.0f);
    music.setVolume(masterVolume);
    update();
    for (auto& activeSound : sounds) {
        activeSound.sound.setVolume(masterVolume);
    }
}

float AudioManager::getMasterVolume() const { return masterVolume; }

void AudioManager::clear() {
    sounds.clear();
    music.stop();
    buffers.clear();
}
