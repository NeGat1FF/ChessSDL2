#include <SDL2/SDL.h>

#include "Board/Board.h"
#include "TextureManager/TextureManager.h"

int main(int argc, char *argv[]){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("SDL2 Window",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SQUARE_SIZE * 8, SQUARE_SIZE * 8, SDL_WINDOW_VULKAN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    if (!TextureManager::Instance().Load("../assets/sprites", renderer)){
        return 1;
    }

    SDL_Surface *icon = IMG_Load("../assets/icon/icon.ico");

    SDL_SetWindowIcon(window, icon);

    Board board;

    board.InitPieces();

    while(true){
        SDL_Event e;
        if(SDL_PollEvent(&e)){
            if(e.type == SDL_MOUSEBUTTONDOWN){
                board.Click(e.button.x, e.button.y);
            }
            if(e.type == SDL_KEYDOWN){
                if(e.key.keysym.sym == SDLK_SPACE){
                    // SDL_Log(board.GetFEN().c_str());
                }
            }
            if(e.type == SDL_QUIT){
                break;
            }
        }

        board.Draw(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}