// TextureManager.cpp
#include "TextureManager/TextureManager.h"

bool TextureManager::Load(const std::string& id, const std::string& filename, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(filename.data());

    if (!surface) {
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        return false;
    }

    textures[id] = texture;
    return true;
}

bool TextureManager::Load(const std::string& dir, SDL_Renderer* renderer){

    if(!std::filesystem::exists(dir)){
        return false;
    }

    for(const auto& entry : std::filesystem::directory_iterator(dir)){
        std::string path = entry.path().string();
        std::string id = entry.path().filename().string();
        Load(id, path, renderer);
    }

    return true;
}

void TextureManager::Draw(const std::string& id, int x, int y, int width, int height, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect = {0, 0, width, height};
    SDL_Rect destRect = {x, y, width, height};
    SDL_RenderCopyEx(renderer, textures[id], &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::Draw(const std::string& id, const SDL_Rect* rect, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(renderer, textures[id], nullptr, rect, 0, nullptr, flip);
}

void TextureManager::Cleanup() {
    for (auto& texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}
