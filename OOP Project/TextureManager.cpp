#include <iostream>
#include <SDL_image.h>

#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture, SDL_Renderer* ren){

    //Initialize PNG loading
    SDL_Surface* tempSurface = IMG_Load(texture);
    if (tempSurface == NULL){
        std::cout <<  "Unable to load image.! SDL_image Error: \n" <<  IMG_GetError();
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
    SDL_FreeSurface(tempSurface);  //does not follow RAII
    return tex;
}

SDL_Point TextureManager::getsize(SDL_Texture* texture){
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}
