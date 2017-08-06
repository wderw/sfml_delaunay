#include "Vector.h"

Vector::Vector()
{
	x = 0;
	y = 0;
}

Vector::Vector(double x, double y)
{
	this->x = x;
	this->y = y;
}

Vector Vector::operator+(const Vector & other)
{
	Vector result;
	result.x = this->x + other.x;
	result.y = this->y + other.y;
	return result;
}

Vector Vector::operator-(const Vector & other)
{
	Vector result;
	result.x = this->x - other.x;
	result.y = this->y - other.y;
	return result;
}

