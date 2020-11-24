#pragma once

#include <SDL.h>
#include "EntityManager.h"

//this is the main class of the game
class Game{
private:
	bool isRunning;
public:
	//Game variables
	SDL_Window* window;
	SDL_Renderer* renderer;
	EntityManager entityManager;
	static SDL_Event event;

	Game();
	Game(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);
	~Game();
	
	//fuction used to initiate the SDL componets we require
	void init(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);
	void handleEvents();
	void render();
	void update(float frameTime);

	inline bool running() { return isRunning; }
};

