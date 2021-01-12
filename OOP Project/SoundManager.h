#pragma once

#include <iostream>

#include "AssetManager.h"

//!O clasa care foloseste SDP pentru a face sunet.
class SoundManager {

private:
	static SoundManager* sInstance;

	AssetManager* assetManager;
public:
	static SoundManager* Instance();
	static void Release();

	//!Aici gestionam melodi lungi care sunt citite din memorie
	void PlayMusic(std::string filename, int loops);
	void PauseMusic();
	void ResumeMusic();

	//!Aici gestionam efectele sonore scurte, care sunt stocate in ram
	void PlaySound(std::string filename, int loop);

private:
	SoundManager();
	~SoundManager();
};