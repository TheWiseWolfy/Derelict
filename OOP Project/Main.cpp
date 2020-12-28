//If something is worth doing, it's worth doing poorely 

#include <chrono>
#include <thread>
#include "Game.h"

float frameStart;
float frameTime = 0;

int main(int argc, char* args[]) {

	Game game("Derelict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	//Main game loop
	while (game.running()) {

		frameStart = SDL_GetTicks();  //since SDL initiation in miliseconds

		game.handleEvents();
		game.update(frameTime);
		game.render();

		
		std::this_thread::sleep_for(std::chrono::milliseconds(16));  //creating a teoretical max FPS of 1000 for stability reasons, not very portable


		frameTime = SDL_GetTicks() - frameStart;


		//Runtime debug info
		std::cout << "FPS: "<<1000 / frameTime;
		std::cout << "\r";

		//std::cout <<"Frames per second"<< 1000/frameTime<< "\n";
	}

	return 0;
}

