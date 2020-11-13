#pragma once
#include <SDL.h>

class GameObject{
private:
	int xpos;
	int ypos;

	SDL_Texture* objTexture;
	SDL_Rect srcRect , destRect;
	SDL_Renderer* renderer;
public:
	GameObject(const char* texturesheet, int x, int y);
	~GameObject();

	void Update();
	void Render();

};

