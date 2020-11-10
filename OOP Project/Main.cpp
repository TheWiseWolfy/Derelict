#include "Game.h"

Uint32 frameStart;
int frameTime;

int main(int argc, char* args[]) {

	Game game("Derelict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	//Main game loop
	while (game.running()) {

		frameStart = SDL_GetTicks();

		game.handleEvents();
		game.update();
		game.render();

		frameTime = SDL_GetTicks() -frameTime;
	}


	return 0;
}