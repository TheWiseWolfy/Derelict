#pragma once

#include <SDL.h>

#include "EntityManager.h"
#include "Vector2D.h"
#include "AssetManager.h"
#include "SoundManager.h"

//asta e clasa principala a jocului
class Game{
private:
	bool isRunning;               //variabila de control pentru inchiderea jocului
	bool lost = false;			

	int stage = 1;
	Entity* player = nullptr;
public:
	//variabile SDL
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	
	static EntityManager entityManager;      //aici sunt stocate toate obiectele jocului
	static int score;					

	AssetManager* assetManager;				//incarcam resurse inteligent
	SoundManager* soundManager;

	Game(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);
	~Game();
	
	//initializam fereastra si inca cateva variabile importante
	void init(const char* title, int xpos, int ypos, int width, int heigh, bool fullscreen);

	//Gestioneaza evenimente SDL
	void handleEvents();
	//Desenam toata obiectele jocului si elemente din HUD
	void render();
	//Actualizam logica tuturor obiectelor
	void update(float frameTime);

	//Definim starea initiala a nivelului
	void setInitialState();		
	//Desenam pe ecran elemente din HUD
	void drawHUD();	
	//Resetam scena si variabile din game
	void reset();						
	void youLostMessage();		
	//Generam asteroizi folosing perlin noise
	void asteroidGeneration();				
	//Cream inamici noi in centrul hartii
	void enemySpawn(int n, Entity* target);		

	inline bool running() { return isRunning; }
};

