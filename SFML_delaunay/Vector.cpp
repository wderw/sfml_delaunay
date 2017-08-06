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

Vector Vector::operator*(int scalar)
{
	return Vector(this->x*scalar,this->y*scalar);
}

double Vector::DotProduct(const Vector& other)
{
	return (this->x*other.x) + (this->y*other.y);
}

double Vector::DotProduct(Vector& v1, Vector& v2)
{
	return (v1.x*v2.x) + (v1.y*v2.y);
}

double Vector::Magnitude()
{
	return sqrt(x*x + y*y);
}

double Vector::DistanceTo(Vector& other)
{
	Vector differenceVector = other - *this;
	return differenceVector.Magnitude();
}

void Vector::Normalize()
{
	double length = this->Magnitude();
	this->x /= length;
	this->y /= length;
}

Vector Vector::Normalized()
{
	double length = this->Magnitude();	
	return Vector(this->x / length, this->y / length);
}

double Vector::AngleBetween(Vector& v1, Vector& v2)
{
	Vector v1_n = v1.Normalized();
	Vector v2_n = v2.Normalized();
	return acos(Vector::DotProduct(v1_n, v2_n));
}

double Vector::AngleTo(Vector & other)
{
	Vector v1_n = this->Normalized();
	Vector v2_n = other.Normalized();
	return acos(Vector::DotProduct(v1_n, v2_n));
}
