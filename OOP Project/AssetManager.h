#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

class AssetManager{
private:
	static AssetManager* sInstance;

	//Pastreaza texturile pe care le-am incarcat
	std::map <std::string, SDL_Texture*> textures;
	//Pastreaza fisierele audio de durata mai lunga
	std::map <std::string, Mix_Music*> songs;
	//Pastreaza fisierele audio de durata mai scurta
	std::map <std::string, Mix_Chunk*> sounds;


public:
	//O fuctie care asigura ca daca inca nu exista o instanta, va exista
	static AssetManager* Instance();
	//Eliberam memoria
	static void Relase();

	//Recupereaza eficient o textura
	SDL_Texture* GetTexture(std::string filename);
	//Recupereaza eficient o melodie
	Mix_Music* GetMusic(std::string filename);
	//Recupereaza eficient un efect sonor
	Mix_Chunk* GetSoundEffects(std::string filename);

private:
	AssetManager();
	~AssetManager();
	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* ren);

};

