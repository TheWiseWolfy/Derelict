// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <string>
#include <SDL_ttf.h>

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
int Game::score = 0;

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

	assetManager->Relase();
	soundManager->Release();

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
	if (TTF_Init() < 0) {
		std::cout << "STD_TTF failed to initialize";
	}

	//Aici setam marimea camerei ca fiind marimea ferestrei pe care o folosim
	Level::camera_size.x = width;
	Level::camera_size.y = heigh;

	//We define important things
	assetManager = AssetManager::Instance();
	if (assetManager == nullptr) {
		return;
	}

	soundManager = SoundManager::Instance();
	if (soundManager == nullptr) {
		return;
	}
	//Aici setam parametri jocului, cream toate obiectele
	setInitialState();

}
void Game::reset() {
	stage = 1;
	score = 0;
	entityManager.clear();
	Level::activeEnemies = 1;
	lost = false;
	player = nullptr;
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

	//Daca nu am pierdut jocul
	if (!lost) {
		//Actualizam fiecare entitate
		entityManager.update(frameTime);

		//Daca nu mai sunt inamici, crean mai multi inamici
		if (Level::activeEnemies == 0) {
			stage++;
			enemySpawn(stage, player);
			Level::activeEnemies = stage;
		}
		if (stage == 5) {
			//You won
		}
		if (player->getComponent<PlayerComponent>().life <= 0) {
			lost = true;
		}
	}
	else if (lost) {
		if (Game::event.type == SDL_KEYDOWN) {
			switch (Game::event.key.keysym.sym) {
			case (SDLK_r):
				reset();
				break;
			case (SDLK_x):
				isRunning = false;
				break;
			default:
				break;
			}
		}
	}
}

void Game::render(){
	SDL_RenderClear(renderer);

	if (!lost) {
		entityManager.draw();
		drawHUD();
	}
	else if (lost) {
		youLostMessage();
	}

	SDL_RenderPresent(renderer);
}
void Game::setInitialState(){

	soundManager->PlayMusic("assets/music1.wav",0);

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
	player = &local_player;
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

	enemySpawn(1, &local_player);
	Level::activeEnemies = 1;
}
void Game::drawHUD(){

	//Draw life bar

	int life = player->getComponent<PlayerComponent>().life;

	SDL_Texture* objTexture = AssetManager::Instance()->GetTexture("assets/health_bar.png");
	SDL_Rect srcRect, destRect;

	int orizontalSize = 0;
	int verticalSize = 0;

	SDL_QueryTexture(objTexture, NULL, NULL, &orizontalSize, &verticalSize);

	//We define the map as the entire image.
	srcRect.h = verticalSize;
	srcRect.w = orizontalSize;
	srcRect.x = 0;
	srcRect.y = 0;

	//We place it in the correct spot
	destRect.h = verticalSize * 4;
	destRect.w = orizontalSize * 4;
	destRect.x = 50;
	destRect.y = 600;

	for (int i = 0 ; i < life ; ++i){

		destRect.x = i * 30 + 50;

		if (SDL_RenderCopyEx(Game::renderer, objTexture, &srcRect, &destRect, 0,
			NULL, SDL_FLIP_NONE) != 0)
		{
			std::cout << SDL_GetError() << '\n';
		}
	}
	
	//Writing text on screen
	std::string toDisplay = "Score: ";
	toDisplay += std::to_string(score);

	TTF_Font* Font = TTF_OpenFont("Sans.ttf", 33); //this opens a font style and sets a size
	if (Font == nullptr) {
		std::cout << TTF_GetError();
	}

	SDL_Color Color = { 255, 255, 255 }; 
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, toDisplay.c_str(), Color); 
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); 

	orizontalSize = 0;
	verticalSize = 0;

	SDL_QueryTexture(Message, NULL, NULL, &orizontalSize, &verticalSize);

	SDL_Rect Message_rect;
	Message_rect.x = 0;  
	Message_rect.y = 0; 
	Message_rect.w = orizontalSize; 
	Message_rect.h = verticalSize; 

	TTF_CloseFont(Font);
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect); 
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	
}

void Game::youLostMessage(){
	//Writing text on screen
	std::string toDisplay = "You lost (R to starg again)";

	TTF_Font* Font = TTF_OpenFont("Sans.ttf", 77); //this opens a font style and sets a size
	if (Font == nullptr) {
		std::cout << TTF_GetError();
	}

	SDL_Color Color = { 0, 0, 0 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, toDisplay.c_str(), Color);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

	int orizontalSize = 0;
	int verticalSize = 0;

	SDL_QueryTexture(Message, NULL, NULL, &orizontalSize, &verticalSize);

	SDL_Rect Message_rect;
	Message_rect.x = 640 - orizontalSize / 2;
	Message_rect.y = 400 - verticalSize / 2;
	Message_rect.w = orizontalSize;
	Message_rect.h = verticalSize;



	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

	TTF_CloseFont(Font);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
	SDL_DestroyTexture(Message);


}
void Game::asteroidGeneration() {

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
			if (fPerlinNoise2D[i * nOutput + j] > 0.67f) {

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
	}
}
void Game::enemySpawn(int n,Entity* target){

	Wireframe vecModelEnemy = {
	{-50.0,0},
	{30.0f,-50.0f},
	{30.0f,50.0f}
	};

	for(int i = 0; i < n ; i++){
		auto& enemy = entityManager.addEntity();
		auto& enemy_tranfsorm(enemy.addComponent<Transform>(new Vector2D(5200 + 200 * i, 4800), M_PI / 2, 7.0f)  /**/);
		auto& enemy_firearm(enemy.addComponent<FirearmComponent>(enemy_tranfsorm)  /**/);
		auto& enemy_sprite(enemy.addComponent<SimpleSprite>(enemy_tranfsorm, "assets/enemy_ship.png", 100, 100, -90)  /**/);
		auto& enemy_colider(enemy.addComponent<EnemyCollider>(enemy_tranfsorm, vecModelEnemy));
		auto& enemy_AI(enemy.addComponent<EnemyComponent>(*target, enemy_tranfsorm));
	}
}



