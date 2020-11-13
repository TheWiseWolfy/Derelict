#pragma once

#include <SDL.h>

//this is the main class of the game
class Game{
private:

	bool isRunning;
	SDL_Window *window;
public:
	static SDL_Renderer* renderer;

	Game();
	Game(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);
	~Game();
	
	//fuction used to initiate the SDL componets we require
	void init(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);
	void handleEvents();
	void render();
	void update(int frameTime);

	inline bool running() { return isRunning; }
};

