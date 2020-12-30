#include "Vector2D.h"

Vector2D::Vector2D(float x, float y){
	this->x = x;
	this->y = y;
}

Vector2D Vector2D::operator+=(const Vector2D& v){
	this->x += v.x;
	this->y += v.y;
	return *this;
}

Vector2D operator*(const Vector2D& v,float scalar){
	Vector2D temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	return temp;
}
Vector2D operator*( float scalar, const Vector2D& v){
	Vector2D temp;
	temp.x = v.x * scalar;
	temp.y = v.y * scalar;
	return temp;
}

Vector2D operator+(const Vector2D& v1, const Vector2D& v2){
	Vector2D temp;
	temp.x = v1.x + v2.x;
	temp.y = v1.y + v2.y;
	return temp;
}

std::ostream& operator<<(std::ostream& stream ,const Vector2D& v ){
	stream << "(" << v.x << ", " << v.y << " )";
	return stream;
}
