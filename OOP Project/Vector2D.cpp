#include "Vector2D.h"

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D Vector2D::operator+(const Vector2D& v)
{
	Vector2D temp;
	temp.x = this->x + v.x;
	temp.y = this->y + v.y;
	return temp;
}

std::ostream& Vector2D::operator<<(std::ostream& stream)
{
	stream << "(" << this->x << ", " << this->y << " )";
	return stream;
}
