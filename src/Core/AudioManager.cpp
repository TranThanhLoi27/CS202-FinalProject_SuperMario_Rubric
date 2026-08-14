#include<Core/AudioManager.h>

bool AudioManager::loadSound(const std::string& id, const std::string& path){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile(path))return false;
    buffers[id] = std::move(buffer);
    return true;
}

void AudioManager::play(const std::string& id){
    const auto it = buffers.find(id);
    if(it == buffers.end())return;
    sounds.emplace_back(it->second);
    sounds.back().setVolume(masterVolume);
    sounds.back().play();
}

bool AudioManager::playMusic(const std::string& path, bool loop){
    if(!music.openFromFile(path)){
        return false;
    }
    music.setLooping(loop);
    music.setVolume(masterVolume);
    music.play();
    return true;
}

void AudioManager::update(){
    for(auto it = sounds.begin(); it != sounds.end(); ){
        if(it->getStatus() == sf::Sound::Status::Stopped){
            it = sounds.erase(it);
        }
        else ++it;
    }
}

void AudioManager::setMasterVolume(float volume){
    masterVolume = volume;
    music.setVolume(masterVolume);
    update();
    for(auto& sound: sounds){
        sound.setVolume(volume);
    }
}

float AudioManager::getMasterVolume() const { return masterVolume; }

void AudioManager::clear(){
    buffers.clear();
    sounds.clear();
    music.stop();
}
