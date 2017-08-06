#pragma once

#include <cmath>

// 2D Vector
class Vector
{
public:
	double x, y;

	Vector();
	Vector(double, double);	

	Vector operator+(const Vector& other);
	Vector operator-(const Vector& other);
	Vector operator*(int scalar);

	double DotProduct(const Vector& other);
	static double DotProduct(Vector& v1, Vector& v2);

	double Magnitude();
	double DistanceTo(Vector& other);
		
	void Normalize();
	Vector Normalized();

	static double AngleBetween(Vector& v1, Vector& v2);
	double AngleTo(Vector& other);
};
