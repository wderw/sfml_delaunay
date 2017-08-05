#pragma once

#include "common.h"
#include "IRenderable.h"

class Vertex// : public IRenderable
{
public:
	static std::vector<Vertex*> vertices;

	sf::Vector2<double> position;
	sf::CircleShape shape;
	sf::Color fillColor;
	sf::Color outlineColor;

	Vertex();
	Vertex(double, double);

	inline void moveTo(double, double);
	inline void moveTo(sf::Vector2<double>);
	void moveBy(double, double);

	//virtual void render(sf::RenderWindow *window);
};