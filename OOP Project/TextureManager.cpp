#include <iostream>
#include <SDL_image.h>

#include "TextureManager.h"

#include "Game.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture){

    //Initialize PNG loading
 
    SDL_Surface* tempSurface = IMG_Load(texture);

    if (tempSurface == NULL)
    {
        std::cout <<  "Unable to load image.! SDL_image Error: \n" <<  IMG_GetError();
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    
    SDL_FreeSurface(tempSurface);  //does not follow RAII
    return tex;
}
