#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

class AssetManager{
private:
	static AssetManager* sInstance;

	std::map <std::string, SDL_Texture*> textures;
	std::map <std::string, Mix_Music*> songs;
	std::map <std::string, Mix_Chunk*> sounds;


public:
	static AssetManager* Instance();
	static void Relase();

	SDL_Texture* GetTexture(std::string filename);
	Mix_Music* GetMusic(std::string filename);
	Mix_Chunk* GetSoundEffects(std::string filename);

private:
	AssetManager();
	~AssetManager();

};

