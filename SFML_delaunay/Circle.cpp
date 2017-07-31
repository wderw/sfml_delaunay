#include "Circle.h"

Circle::Circle()
{
	center = sf::Vector2f(0.0f, 0.0f);
	radius = 0.0f;
	if (!(Circle::redColorProgression + 35 > 255))
	{
		Circle::redColorProgression += 35;
	}
}

Circle::Circle(sf::Vector2f center, float radius) : Circle()
{
	this->center = center;
	this->radius = radius;

	fillColor = sf::Color::Transparent;
	outlineColor = sf::Color(Circle::redColorProgression, 225, 120);

	shape.setOutlineThickness(2.0f);

	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);

	this->shape.setRadius(radius);
	this->shape.setPosition(center);

	this->shape.setPointCount(100);
}

void Circle::render(sf::RenderWindow * window)
{
	//this->shape.setOutlineColor(outlineColor);
	//this->shape.setFillColor(fillColor);
	sf::Vector2f offset(shape.getRadius(), shape.getRadius());
	this->shape.setPosition(center - offset);
	window->draw(shape);
}
