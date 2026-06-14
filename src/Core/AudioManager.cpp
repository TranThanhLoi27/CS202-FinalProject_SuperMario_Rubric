#include<Core/AudioManager.h>

bool AudioManager::LoadSound(const std::string& id, const std::string& path){
    sf::SoundBuffer buffer;
    if(!buffer.loadFromFile(path))return false;
    buffers[id] = std::move(buffer);
    return true;
}

void AudioManager::PlaySound(const std::string& id){
    const auto it = buffers.find(id);
    if(it == buffers.end())return;
    sounds.emplace_back(it->second);
    sounds.back().setVolume(mastervolume);
    sounds.back().play();
}

void AudioManager::PlayMusic(const std::string& path, bool loop){
    if(!music.openFromFile(path)){
        return;
    }
    music.setLooping(loop);
    music.setVolume(mastervolume);
    music.play();
}

void AudioManager::Update(){
    for(auto it = sounds.begin(); it != sounds.end(); ){
        if(it->getStatus() == sf::Sound::Status::Stopped){
            it = sounds.erase(it);
        }
        else ++it;
    }
}

void AudioManager::SetVolume(const float& volume){
    mastervolume = volume;
    music.setVolume(mastervolume);
    AudioManager::Update();
    for(auto it: sounds){
        it.setVolume(volume);
    }
}

void AudioManager::Clear(){
    buffers.clear();
    sounds.clear();
    music.stop();
}