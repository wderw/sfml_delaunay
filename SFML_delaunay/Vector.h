#pragma once

class Vector
{
public:
	double x, y;

	Vector();
	Vector(double, double);	

	Vector operator+(const Vector& other);
	Vector operator-(const Vector& other);
};
