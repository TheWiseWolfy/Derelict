#include <iostream>

#include "GameObject.h"

#include "TextureManager.h"
#include "Game.h";

GameObject::GameObject(const char* texturesheet,int x,int y){
	renderer = Game::renderer;
	objTexture = TextureManager::LoadTexture(texturesheet);

	xpos = x;
	ypos = y;
}

GameObject::~GameObject(){
	delete(objTexture);
	objTexture = nullptr;
}

void GameObject::Update(){
	xpos = 0;
	xpos = 0;

	srcRect.h = 500;
	srcRect.w = 500;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.h = 500;
	destRect.w = 500;
	destRect.x = 0;
	destRect.y = 0;
}

void GameObject::Render(){

	if (SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect) !=0) {
		std::cout << SDL_GetError()<< '\n';
	}
}
