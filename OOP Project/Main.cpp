#include "Game.h"


int main(int argc, char* args[]) {

	Game game("Derelict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
	//Main game loop
	while (game.running()) {
		game.handleEvents();
		game.update();
		game.render();
	}


	return 0;
}