#pragma once

#include <iostream>
#include "AssetManager.h"


class SoundManager {

private:
	static SoundManager* sInstance;

	AssetManager* assetManager;
public:
	static SoundManager* Instance();
	static void Release();

	//Aici gestionam melodilungi care sunt citite din memorie
	void PlayMusic(std::string filename, int loops);

	//Aici gestionam efectele sonore scurte, care sunt stocate in ram
	void PlaySound(std::string filename, int loop);
	void PauseSound();
	void ResumeSound();


private:
	SoundManager();
	~SoundManager();
};