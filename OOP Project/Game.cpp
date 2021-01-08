// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>

#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Coliders.h"
#include "LevelManager.h"


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

	//Initializari SDL
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

	//Aici setam marimea camerei ca fiind marimea ferestrei pe care o folosim
	Level::camera_size.x = width;
	Level::camera_size.y = heigh;


	//Aici setam parametri jocului, cream toate obiectele
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
	{30.0f,-50.0f},
	{30.0f,50.0f}
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
	auto& player_tranfsorm(local_player.addComponent<Transform>(new Vector2D(5050,5050), M_PI/2,5.0f)  /**/);
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

	//Generate first enemy

	auto& enemy = entityManager.addEntity();
	auto& enemy_tranfsorm(enemy.addComponent<Transform>(new Vector2D(5200, 4800), M_PI / 2, 7.0f)  /**/);
	auto& enemy_firearm(enemy.addComponent<FirearmComponent>(enemy_tranfsorm)  /**/);
	auto& enemy_sprite(enemy.addComponent<SimpleSprite>(enemy_tranfsorm, "assets/enemy_ship.png", 100, 100, -90)  /**/);
	auto& enemy_colider(enemy.addComponent<EnemyCollider>(enemy_tranfsorm, vecModelShip));
	auto& enemy_AI(enemy.addComponent<EnemyComponent>(local_player,enemy_tranfsorm));
}

void  Game::asteroidGeneration() {

	Wireframe vecModelAsteroid = {
	{-45.0,0},
	{-30.0,-40.0},
	{20.0f,-30.0f},
	{40.0f,30.0f},
	{20.0f,45.0f}
	};

	Wireframe updatedModelAsteroid = vecModelAsteroid;

	float* fNoiseSeed2D = nullptr;
	float* fPerlinNoise2D = nullptr;

	int nOutput = 36;

	fNoiseSeed2D = new float[nOutput * nOutput];
	fPerlinNoise2D = new float[nOutput * nOutput];

	for (int i = 0; i < nOutput * nOutput; i++){
		fNoiseSeed2D[i] = (float)rand() / (float)RAND_MAX;
	}

	PerlinNoise2D(nOutput, nOutput, fNoiseSeed2D, 6, 0.5, fPerlinNoise2D);

	int generationDensity = 36;

	float sectorHeigh = Level::levelHeigh / generationDensity;
	float sectorWidth = Level::levelWidth / generationDensity;
	for (size_t i = 0; i < generationDensity; i++)
	{
		for (size_t j = 0; j < generationDensity; j++)
		{
			if (fPerlinNoise2D[i * nOutput + j] > 0.65f) {

				auto& asteroid = entityManager.addEntity();

				float randomRotation= (float)rand() / (float)RAND_MAX;
				float randomPozition = (float)rand() / (float)RAND_MAX;
				float randomScale = (float)rand() / (float)RAND_MAX;

				for (int f1 = 0; f1 < vecModelAsteroid.size(); f1++) {
					updatedModelAsteroid[f1].first = vecModelAsteroid[f1].first * randomScale  + vecModelAsteroid[f1].first;
					updatedModelAsteroid[f1].second = vecModelAsteroid[f1].second * randomScale + vecModelAsteroid[f1].second;
				}

				auto& asteroid_tranfsorm(asteroid.addComponent<Transform>(new Vector2D(j * sectorHeigh + sectorHeigh * randomPozition,  i * sectorWidth + sectorWidth * randomPozition), M_PI * randomRotation, randomScale * 10.0f + 5.0f)  /**/);
				auto& asteroid_sprite(asteroid.addComponent<SimpleSprite>(asteroid_tranfsorm, "assets/asteroid_1.png", randomScale * 100 + 100, randomScale*100 + 100, -90)  /**/);
				auto& asteroid_colider(asteroid.addComponent<AsteroidCollider>(asteroid_tranfsorm, updatedModelAsteroid));
			}
		}
		std::cout << " \n";
	}

}


