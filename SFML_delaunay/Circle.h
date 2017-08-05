#pragma once

#include "common.h"
#include "IRenderable.h"

class Circle : public IRenderable
{
public:
	static int redColorProgression;

	sf::CircleShape shape;
	sf::Color fillColor;
	sf::Color outlineColor;

	sf::Vector2<double> center;
	double radius;

	Circle();
	Circle(sf::Vector2<double> center, double radius);
	virtual void render(sf::RenderWindow *window);
};