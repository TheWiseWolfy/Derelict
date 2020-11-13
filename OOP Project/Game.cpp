#include <iostream>
#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h";

GameObject* test;

SDL_Renderer* Game::renderer = nullptr;

Game::Game()
{
	 init("Derelict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
}

Game::Game(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen){
	init(title, xpos, ypos, width, heigh, fullscreen);
}

Game::~Game()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "\nGame Cleanned Succesfully" << '\n';
}

void Game::init(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "\n**Initialization Start**";

		window = SDL_CreateWindow(title, xpos, ypos, width, heigh, flags);
		if (window) {
			std::cout << "\nWindows Created!";
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "\nRenderer Created!";
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}

	test = new GameObject("assets/index.png", 0 ,0);

}

void Game::handleEvents(){
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update(int frameTime) {
	test->Update();
}


void Game::render(){
	SDL_RenderClear(renderer);
	test->Render();
	SDL_RenderPresent(renderer);
}



