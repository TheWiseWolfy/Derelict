#include "SoundManager.h"

SoundManager* SoundManager::sInstance = nullptr;

SoundManager* SoundManager::Instance()
{
	if (sInstance == nullptr) {
		sInstance = new  SoundManager();
	}
	return sInstance;
}

void SoundManager::Release(){
	delete sInstance;
	sInstance = nullptr;
}

void SoundManager::PlayMusic(std::string filename, int loops) {

	Mix_PlayMusic( assetManager->GetMusic(filename), loops);
}


void SoundManager::PlaySound(std::string filename, int loops){

	Mix_PlayChannel(-1,assetManager->GetSoundEffects(filename),loops);
}

void SoundManager::PauseSound(){

	if (Mix_PlayingMusic() != 0) {
		Mix_PausedMusic();
	}

}
void SoundManager::ResumeSound(){
	if (Mix_PausedMusic() != 0) {
		Mix_ResumeMusic();
	}

}
SoundManager::SoundManager(){
	assetManager = AssetManager::Instance();

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {

		std::cout << "Mixer initialization ERROR: " << Mix_GetError() << "\n";
	}
}

SoundManager::~SoundManager(){
	assetManager = nullptr;
	Mix_Quit();
}
