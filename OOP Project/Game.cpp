#include <iostream>

#include "Game.h"
#include "TextureManager.h"
#include "Components.h"

//Important handles
SDL_Window* Game::window;
SDL_Renderer* Game::renderer;
EntityManager Game::entityManager;
SDL_Event Game::event;
Vector2D Level::camera_position;
Vector2D Level::camera_size;

Game::Game(){
	 init("Derelict", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
}

Game::Game(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen){
	init(title, xpos, ypos, width, heigh, fullscreen);
}

Game::~Game(){
	SDL_DestroyWindow(window);
	
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	std::cout << "\nGame Cleanned Succesfully" << '\n';
}

void Game::init(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen){
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
		std::cout << "\nInitialization Failed!";

	}
	
	Level::camera_size.x = width;
	Level::camera_size.y = heigh;


	//We prepare the actual game
	setInitialState();

}

void Game::handleEvents(){

	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
	case SDLK_ESCAPE:
		break;
	}
}

void Game::update(float frameTime) {

	entityManager.update(frameTime);

}

void Game::render(){
	SDL_RenderClear(renderer);

	entityManager.draw();



	SDL_RenderPresent(renderer);
}

void Game::setInitialState(){

	 //We create an entity and get a reference to it:
	auto& background(entityManager.addEntity());
	auto& background_tranfsorm(background.addComponent<TranformComponent>(new Vector2D(0, 0), 0)/**/ );
	auto& background_sprite(background.addComponent<SimpleSprite>(background_tranfsorm, "assets/galaxy.jpg", 6000, 7000) /**/);

    auto& local_player = entityManager.addEntity();
	auto& player_tranfsorm(local_player.addComponent<TranformComponent>(new Vector2D(300.0f, 300.0f), M_PI/2)  /**/);
	auto& controler(local_player.addComponent<PlayerComponent>(player_tranfsorm)  /**/  );
	auto& sprite(local_player.addComponent<SimpleSprite>(player_tranfsorm, "assets/index.png", 100, 100)  /**/);

	//player = local_player;
	//transform = player_tranfsorm;
}

