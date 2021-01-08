#pragma once

#include "Vector2D.h"

static class Level {
public:
	static float levelWidth;
	static float levelHeigh;
	static int activeEnemies;

	static Vector2D camera_size;
	static Vector2D camera_position;
	static Vector2D screenSpaceToGameSpace(Vector2D screenPosition);
	static Vector2D screenSpaceToGameSpace(int x, int y);
};

void PerlinNoise2D(int nWidth, int nHeight, float* fSeed, int nOctaves, float fBias, float* fOutput);
