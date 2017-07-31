#include "Vertex.h"
#include "Edge.h"

Edge::Edge()
{
	v1 = nullptr;
	v2 = nullptr;

	color1 = sf::Color(CUSTOM_WHITE_ALPHA);
	color2 = sf::Color(CUSTOM_WHITE_ALPHA);
}

Edge::Edge(Vertex * v1, Vertex * v2) : Edge()
{
	this->v1 = v1;
	this->v2 = v2;
}

void Edge::setOrigin(sf::Vector2f origin)
{
	simplex_origin = origin;
}

void Edge::setColor1(sf::Color c)
{
	color1 = c;
}
void Edge::setColor2(sf::Color c)
{
	color2 = c;
}
void Edge::setColor(sf::Color c)
{
	color1 = c;
	color2 = c;
}

void Edge::render(sf::RenderWindow * window)
{
	/*
	sf::Vertex line[] =
	{
	sf::Vertex(v1->position,color1),
	sf::Vertex(v2->position,color2)
	};

	window->draw(line, 2, sf::Lines);
	*/
}
