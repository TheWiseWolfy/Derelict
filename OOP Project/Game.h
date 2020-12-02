#pragma once

#include <SDL.h>
#include "EntityManager.h"
#include "Vector2D.h"

//this is the main class of the game
class Game{
private:
	bool isRunning;
public:
	//Game variables
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static EntityManager entityManager;
	static SDL_Event event;

	Game();
	Game(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);
	~Game();
	
	//fuction used to initiate the SDL componets we require
	void init(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);

	//Game loop fuctions
	void handleEvents();
	void render();
	void update(float frameTime);

	//Auxiliary fuctions
	void setInitialState();
	inline bool running() { return isRunning; }
};

static class Level {
public:
	const float level_width = 10000;
	const float level_heigh = 10000;


	static Vector2D camera_size;

	static Vector2D camera_position;

	static Vector2D screenSpaceToGameSpace(Vector2D screenPosition) {
		return screenPosition + camera_position;
	}

	static Vector2D screenSpaceToGameSpace(int x, int y) {
		 Vector2D temp;
		 temp.x = x - camera_position.x;
		 temp.y = y - camera_position.y;
		 return temp;
	}
};

