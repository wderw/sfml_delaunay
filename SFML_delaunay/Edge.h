#pragma once

#include "common.h"

class Edge : public IRenderable
{
public:
	Vertex *v1, *v2;

	sf::Vector2<double> simplex_origin;

	sf::Color color1;
	sf::Color color2;

	Edge();
	Edge(Vertex *v1, Vertex *v2);

	void setOrigin(sf::Vector2<double>);
	void setColor1(sf::Color);
	void setColor2(sf::Color);
	void setColor(sf::Color);
	virtual void render(sf::RenderWindow *window);
};