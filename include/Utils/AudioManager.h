#pragma once

#include <map>
#include <string>
#include <SDL2/SDL_mixer.h>
#include <filesystem>

class AudioManager {
public:
    static AudioManager& Instance() {
        static AudioManager instance;
        return instance;
    }

    bool LoadSound(const std::string& folderPath);
    bool LoadSound(const std::string& filePath, std::string id);

    bool LoadMusic(const std::string& folderPath);
    bool LoadMusic(const std::string& filePath, std::string id);

    void PlaySound(const std::string& sound, int loops = 0);
    void PlayMusic(const std::string&music, int loops = 0);

    int GetSoundVolume();
    int GetMusicVolume();

    void SetSoundVolume(int volume);
    void SetMusicVolume(int volume);

    void Cleanup();

private:
    AudioManager() {Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);}
    ~AudioManager() { Cleanup(); }

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    std::map<std::string, Mix_Chunk*> _soundCache;
    std::map<std::string, Mix_Music*> _musicCache;

    int _soundVolume = 48;
    int _musicVolume = 48;
};
