#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <filesystem>
#include <regex>
#include <fstream>

class TextureManager {
public:
    static TextureManager& Instance() {
        static TextureManager instance;
        return instance;
    }

    bool Load(const std::string& id, const std::string& filename, SDL_Renderer* renderer);
    bool Load(const std::string& dir, SDL_Renderer* renderer);
    bool LoadSVG(const std::string& id, const std::string& filename, unsigned int size, SDL_Renderer* renderer);
    bool LoadSVG(const std::string& dir,unsigned int size, SDL_Renderer* renderer);

    void Draw(const std::string& id, int x, int y, int width, int height, SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void Draw(const std::string& id, const SDL_Rect* rect, SDL_Renderer* renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void Cleanup();

private:
    TextureManager() {}
    ~TextureManager() { Cleanup(); }

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    std::map<std::string, SDL_Texture*> _textures;
};
