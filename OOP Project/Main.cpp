// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

//If something is worth doing, it's worth doing poorely 

#include <chrono>
#include <thread>
#include "Game.h"

float frameStart;
float frameTime = 0;

int main(int argc, char* args[]) {

	Game game("Derelict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);

	//Main game loop
	while (game.running()) {

		frameStart = SDL_GetTicks();  //since SDL initiation in miliseconds

		game.handleEvents();
		game.update(frameTime);
		game.render();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));  //creating a teoretical max FPS of 1000 for stability reasons, not very portable

		frameTime = SDL_GetTicks() - frameStart;

		//Runtime debug info
	//	std::cout << "FPS: "<<1000 / frameTime;
	//	std::cout << "\r";

	}

	return 0;
}

