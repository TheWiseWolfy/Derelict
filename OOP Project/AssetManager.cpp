
#include <SDL_image.h>
#include <iostream>

#include "AssetManager.h"
#include "Game.h"

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

		textures[filename] = AssetManager::LoadTexture(filename.c_str(), Game::renderer);

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

	//Eliberam toate resursele folosite folosing fuctii specifice

	for (auto tex : textures) {
		if (tex.second != nullptr) {
			SDL_DestroyTexture(tex.second);
		}
	}

	for (auto song : songs) {
		if (song.second != nullptr) {
			Mix_FreeMusic(song.second);
		}
	}

	for (auto sound : sounds) {
		if (sound.second != nullptr) {
			Mix_FreeChunk(sound.second);
		}
	}

	textures.clear();
	songs.clear();
	sounds.clear();
}

SDL_Texture* AssetManager::LoadTexture(const char* texture, SDL_Renderer* ren) {

	//Initialize PNG loading
	SDL_Surface* tempSurface = IMG_Load(texture);
	if (tempSurface == NULL) {
		std::cout << "Unable to load image.! SDL_image Error: \n" << IMG_GetError();
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, tempSurface);
	SDL_FreeSurface(tempSurface);  //does not follow RAII
	return tex;
}
