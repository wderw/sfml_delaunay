#include "Circle.h"

Circle::Circle()
{
	center = sf::Vector2<double>(0.0, 0.0);
	radius = 0.0f;
	if (!(Circle::redColorProgression + 35 > 255))
	{
		Circle::redColorProgression += 35;
	}
}

Circle::Circle(sf::Vector2<double> center, double radius) : Circle()
{
	this->center = center;
	this->radius = radius;

	fillColor = sf::Color::Transparent;
	outlineColor = sf::Color(255,255  ,255, 80);

	shape.setOutlineThickness(0.01f);

	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);

	this->shape.setRadius(radius);
	this->shape.setPosition(sf::Vector2f(center));

	this->shape.setPointCount(100);
}

void Circle::render(sf::RenderWindow * window)
{
	//this->shape.setOutlineColor(outlineColor);
	//this->shape.setFillColor(fillColor);
	sf::Vector2<double> offset(shape.getRadius(), shape.getRadius());
	this->shape.setPosition(sf::Vector2f(center - offset));
	window->draw(shape);
}
