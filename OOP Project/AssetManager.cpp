#include "AssetManager.h"
#include "TextureManager.h"
#include "Game.h"

#include <iostream>

AssetManager* AssetManager::sInstance = nullptr;

AssetManager* AssetManager::Instance(){

	if (sInstance == nullptr) {

		sInstance = new AssetManager();
	}
	return sInstance;
}

void AssetManager::Relase(){
	delete sInstance;
	sInstance = nullptr;
}

SDL_Texture* AssetManager::GetTexture(std::string filename)
{
	if (textures[filename] == nullptr) {

		textures[filename] = TextureManager::LoadTexture(filename.c_str(), Game::renderer);

		if (textures[filename] == nullptr) {
			std::cout << "Error loading texture " << filename << "\n";
		}
	}
	return textures[filename];
}

Mix_Music* AssetManager::GetMusic(std::string filename) {

	if (songs[filename] == nullptr) {

		songs[filename] = Mix_LoadMUS(filename.c_str());
		if (songs[filename] == nullptr) {
			std::cout << "Error loading sound " << filename << ": " << Mix_GetError();
		}
	}
	return songs[filename];
}

Mix_Chunk* AssetManager::GetSoundEffects(std::string filename)
{
	if (sounds[filename] == nullptr) {
		
		sounds[filename] = Mix_LoadWAV(filename.c_str());
		if (sounds[filename] == nullptr) {
			std::cout << "Error loading sound " << filename << ": " << Mix_GetError();
		}
	}
	return sounds[filename];
}

AssetManager::AssetManager(){
}

AssetManager::~AssetManager(){
	for (auto tex : textures) {
		if (tex.second != nullptr) {
			SDL_DestroyTexture(tex.second);
		}
	}

	textures.clear();
	sounds.clear();
}
