#include "Utils/TextureManager.h"

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

    _textures[id] = texture;
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

bool TextureManager::LoadSVG(const std::string& id, const std::string& filename, unsigned int size, SDL_Renderer* renderer){
    // Read file and store it in a string
    std::ifstream file(filename);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    std::string _size = std::to_string(size);

    // Change the size of the svg
    std::regex width_regex(R"(\bwidth="\d+(\.\d+)?\w*")");
    std::regex height_regex(R"(\bheight="\d+(\.\d+)?\w*")");
    std::string updated_content = std::regex_replace(content, width_regex, "width=\"" + _size + "\"");
    updated_content = std::regex_replace(updated_content, height_regex, "height=\"" + _size + "\"");

    // Load from raw data
    SDL_RWops* rw = SDL_RWFromConstMem(updated_content.c_str(), static_cast<int>(updated_content.size()));
    SDL_Surface* surface = IMG_LoadSVG_RW(rw);

    if (!surface) {
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (!texture) {
        return false;
    }

    _textures[id] = texture;
    return true;
}

bool TextureManager::LoadSVG(const std::string& dir,unsigned int size, SDL_Renderer* renderer){
    
        if(!std::filesystem::exists(dir)){
            return false;
        }
    
        for(const auto& entry : std::filesystem::directory_iterator(dir)){
            std::string path = entry.path().string();
            std::string id = entry.path().stem().string();
            LoadSVG(id, path, size, renderer);
        }

        // Get number of files in directory
        std::size_t num_files = std::distance(std::filesystem::directory_iterator(dir), std::filesystem::directory_iterator{});

        if(_textures.size() != num_files)
            return false;
    
        return true;
}

void TextureManager::Draw(const std::string& id, int x, int y, int width, int height, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect = {0, 0, width, height};
    SDL_Rect destRect = {x, y, width, height};
    SDL_RenderCopyEx(renderer, _textures[id], &srcRect, &destRect, 0, nullptr, flip);
}

void TextureManager::Draw(const std::string& id, const SDL_Rect* rect, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(renderer, _textures[id], nullptr, rect, 0, nullptr, flip);
}

void TextureManager::Cleanup() {
    for (auto& texture : _textures) {
        SDL_DestroyTexture(texture.second);
    }
    _textures.clear();
}
