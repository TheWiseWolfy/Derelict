#include "LevelManager.h"

Vector2D Level::screenSpaceToGameSpace(Vector2D screenPosition) {
	return screenPosition + camera_position;
}
Vector2D Level::screenSpaceToGameSpace(int x, int y) {
	Vector2D temp;
	temp.x = x - camera_position.x;
	temp.y = y - camera_position.y;
	return temp;
}
