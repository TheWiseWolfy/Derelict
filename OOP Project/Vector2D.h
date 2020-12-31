#pragma once

#include <iostream>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D() : x(0.0f) , y(0.0f){};
	Vector2D(float x, float y);

	Vector2D operator+=(const Vector2D& v);


	//Scalar multiplication
	friend Vector2D operator*(const Vector2D& v, float scalar);
	friend Vector2D operator*(float scalar, const Vector2D& v);
	friend Vector2D operator+(const Vector2D& v1, const Vector2D& v2);
	friend Vector2D operator-(const Vector2D& v1, const Vector2D& v2);


	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& v);


	 float getMagnitude() {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	 Vector2D normalizeVector();
};

