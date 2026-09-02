// Loads and plays optional sound effects and background music.
#pragma once

#include "Core/SoundObserver.h"

#include <SFML/Audio.hpp>
#include <list>
#include <map>
#include <string>
#include <utility>

class AudioManager : public SoundObserver {
public:
    /// Loads and caches a sound effect without interrupting existing sounds.
    bool loadSound(const std::string& id, const std::string& path);
    /// Opens a streamed music track and starts playback.
    bool playMusic(const std::string& path, bool loop = true);
    /// Plays one cached sound effect by logical identifier.
    void play(const std::string& id);
    /// Stops all active instances of the requested sound effect.
    void stop(const std::string& id);
    /// Stops the streamed background music.
    void stopMusic();
    /// Sets the volume used by music and active or future sound effects.
    void setMasterVolume(float volume);
    /// Returns the current master volume in the range zero to one hundred.
    float getMasterVolume() const;
    /// Removes sound instances that have finished playing.
    void update();
    /// Stops playback and releases all cached audio resources.
    void clear();
    /// Handles a sound request published through the Observer interface.
    void onSoundRequested(const std::string& soundId) override;

private:
    struct ActiveSound {
        /// Couples a logical identifier with one independently playable sound instance.
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
