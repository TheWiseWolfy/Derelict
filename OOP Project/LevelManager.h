#pragma once

#include "Vector2D.h"

//!O clasa care pastreaza detalii despre nivel, camera si interactiunea acestora.
class Level {
public:
	//!Marimea orizontala a nivelului
	static float levelWidth;
	//!Marimea verticala a nivelului
	static float levelHeigh;
	//!Numarul de inamici activi in nivel
	static int activeEnemies;

	//!Marimea camerei, coreleaza cu marimea ferestrei jocului
	static Vector2D camera_size;
	//!Pozitia camerei in lumea jocului
	static Vector2D camera_position;

	//!O fuctie care atribuie mousului o pozitie in lumea jocului
	static Vector2D screenSpaceToGameSpace(Vector2D screenPosition);
	static Vector2D screenSpaceToGameSpace(int x, int y);

	//!Un algoritm simplu pentru perlin noise
	static void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);
};

