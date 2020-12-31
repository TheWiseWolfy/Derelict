// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Coliders.h"

//Important handles
SDL_Window* Game::window;
SDL_Renderer* Game::renderer;
EntityManager Game::entityManager;
SDL_Event Game::event;
Vector2D Level::camera_position;
Vector2D Level::camera_size;

//We set limits for how big the level is going to be;

float Level::levelWidth = 10000;
float Level::levelHeigh = 10000;

//Some useful abtractions 

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
		std::cout << "**Initialization Start**\n";

		window = SDL_CreateWindow(title, xpos, ypos, width, heigh, flags);
		if (window) {
			std::cout << "Windows Created!\n";
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer Created!\n";
		}
		isRunning = true;
	}
	else {
		isRunning = false;
		std::cout << "Initialization Failed!\n";

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

	//We update individual 
	entityManager.update(frameTime);

}
void Game::render(){
	SDL_RenderClear(renderer);
	entityManager.draw();
	SDL_RenderPresent(renderer);
}
void Game::setInitialState(){

	Wireframe vecModelShip = {
	{-50.0,0},
	{50.0f,-50.0f},
	{50.0f,50.0f}
	};
	Wireframe vecModelStation = {
	{-130.0,50},
	{-130.0,-50},

	{-50,-130.0},
	{50,-130.0},

	{130,-50.0},
	{130,50.0},

	{50.0,130},
	{-50.0,130},
	};


	//BACKGROUND
	auto& background(entityManager.addEntity());
	auto& background_sprite(background.addComponent<StaticSprite>( "assets/galaxy3.png",0,0,720, 2057,0,0) /**/);

	auto& background_station(entityManager.addEntity());
	auto& background_station_sprite(background_station.addComponent<StaticSprite>("assets/station.png", 800, 600 ,60, 100,50, 0) /**/);

	auto& background_ship(entityManager.addEntity());
	auto& background_ship_sprite(background_ship.addComponent<StaticSprite>("assets/background_ship1.png", 600, 400, 76, 270, 30, 0) /**/);
	
	//Set up the player	
    auto& local_player = entityManager.addEntity();
	auto& player_tranfsorm(local_player.addComponent<Transform>(new Vector2D(5000,5000), M_PI/2,2.0f)  /**/);
	auto& controler(local_player.addComponent<PlayerComponent>(player_tranfsorm)  /**/  );
	auto& firearm(local_player.addComponent<FirearmComponent>(player_tranfsorm)  /**/);
	auto& player_sprite(local_player.addComponent<SimpleSprite>(player_tranfsorm, "assets/ship2.png", 100, 100,-90)  /**/);
	auto& player_colider(local_player.addComponent<PlayerCollider>(player_tranfsorm, vecModelShip));

	
	//Local station
	auto& station = entityManager.addEntity();
	auto& station_tranfsorm(station.addComponent<Transform>(new Vector2D(5300.0f, 5200.0f), M_PI / 2,1000.0f)  /**/);
	auto& station_sprite(station.addComponent<SimpleSprite>(station_tranfsorm, "assets/space_station.png", 300, 300, 0)  /**/);
	auto& station_colider(station.addComponent<StationCollider>(station_tranfsorm, vecModelStation));

	asteroidGeneration();

}

void  Game::asteroidGeneration() {

	Wireframe vecModelAsteroid = {
{-45.0,0},
{-30.0,-40.0},

{20.0f,-30.0f},
{40.0f,30.0f},

{20.0f,45.0f}

	};


	auto& asteroid = entityManager.addEntity();
	auto& asteroid_tranfsorm(asteroid.addComponent<Transform>(new Vector2D(5700.0f, 5500.0f), M_PI / 3, 1.0f)  /**/);
	auto& asteroid_sprite(asteroid.addComponent<SimpleSprite>(asteroid_tranfsorm, "assets/asteroid_1.png", 100, 100, -90)  /**/);
	auto& asteroid_colider(asteroid.addComponent<AsteroidCollider>(asteroid_tranfsorm, vecModelAsteroid));

	auto& asteroid2 = entityManager.addEntity();
	auto& asteroid_tranfsorm2(asteroid2.addComponent<Transform>(new Vector2D(5700.0f, 5300.0f), M_PI / 4, 5.0f)  /**/);
	auto& asteroid_sprite2(asteroid2.addComponent<SimpleSprite>(asteroid_tranfsorm2, "assets/asteroid_1.png", 100, 100, -90)  /**/);
	auto& asteroid_colider2(asteroid2.addComponent<AsteroidCollider>(asteroid_tranfsorm2, vecModelAsteroid));

	auto& asteroid3 = entityManager.addEntity();
	auto& asteroid_tranfsorm3(asteroid3.addComponent<Transform>(new Vector2D(5700.0f, 5100.0f), M_PI / 2, 5.0f)  /**/);
	auto& asteroid_sprite3(asteroid3.addComponent<SimpleSprite>(asteroid_tranfsorm3, "assets/asteroid_1.png", 100, 100, -90)  /**/);
	auto& asteroid_colider3(asteroid3.addComponent<AsteroidCollider>(asteroid_tranfsorm3, vecModelAsteroid));

}


Vector2D Level::screenSpaceToGameSpace(Vector2D screenPosition) {
	return screenPosition + camera_position;
}
Vector2D Level::screenSpaceToGameSpace(int x, int y) {
	Vector2D temp;
	temp.x = x - camera_position.x;
	temp.y = y - camera_position.y;
	return temp;
}


