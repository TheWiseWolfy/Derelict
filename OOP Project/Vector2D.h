#pragma once
#include <iostream>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D() : x(0.0f) , y(0.0f){};
	Vector2D(float x, float y);

	Vector2D operator+(const Vector2D& v);

	std::ostream& operator<<(std::ostream& stream);
};

