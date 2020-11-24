#pragma once
#include <SDL.h>

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);

    static SDL_Point getsize(SDL_Texture* texture);

};

