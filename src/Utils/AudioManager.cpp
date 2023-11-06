#include "Utils/AudioManager.h"

bool AudioManager::LoadSound(const std::string& filePath, std::string id){
    Mix_Chunk* chunk = Mix_LoadWAV(filePath.c_str());


    if(!chunk){
        return false;
    }

    Mix_VolumeChunk(chunk, _soundVolume);

    _soundCache[id] = chunk;
    return true;
}

bool AudioManager::LoadSound(const std::string& folderPath){
    if(!std::filesystem::exists(folderPath)){
        return false;
    }

    for(const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::string path = entry.path().string();
        std::string id = entry.path().stem().string();
        LoadSound(path, id);
    }

    // Get number of files in folder
    std::size_t numFiles = std::distance(std::filesystem::directory_iterator(folderPath), std::filesystem::directory_iterator{});

    if(numFiles != _soundCache.size()){
        return false;
    }

    return true;
}

bool AudioManager::LoadMusic(const std::string& filePath, std::string id){
    Mix_Music* music = Mix_LoadMUS(filePath.c_str());

    if(!music){
        return false;
    }

    Mix_VolumeMusic(_musicVolume);

    _musicCache[id] = music;
    return true;
}

bool AudioManager::LoadMusic(const std::string& folderPath){
    if(!std::filesystem::exists(folderPath)){
        return false;
    }

    for(const auto& entry : std::filesystem::directory_iterator(folderPath)){
        std::string path = entry.path().string();
        std::string id = entry.path().filename().string();
        LoadMusic(path, id);
    }

    // Get number of files in folder
    std::size_t numFiles = std::distance(std::filesystem::directory_iterator(folderPath), std::filesystem::directory_iterator{});

    if(numFiles != _soundCache.size()){
        return false;
    }

    return true;
}

void AudioManager::PlaySound(const std::string& sound, int loops){
    Mix_PlayChannel(-1, _soundCache[sound], loops);
}

void AudioManager::PlayMusic(const std::string& music, int loops){
    Mix_PlayMusic(_musicCache[music], loops);
}

int AudioManager::GetSoundVolume(){
    return _soundVolume;
}

int AudioManager::GetMusicVolume(){
    return _musicVolume;
}

void AudioManager::SetSoundVolume(int volume){
    _soundVolume = volume;
    for(auto& sound : _soundCache){
        Mix_VolumeChunk(sound.second, _soundVolume);
    }
}

void AudioManager::SetMusicVolume(int volume){
    _musicVolume = volume;
    Mix_VolumeMusic(_musicVolume);
}

void AudioManager::Cleanup(){
    for(auto& sound : _soundCache){
        Mix_FreeChunk(sound.second);
    }
    _soundCache.clear();

    for(auto& music : _musicCache){
        Mix_FreeMusic(music.second);
    }
    _musicCache.clear();

    Mix_CloseAudio();
}