#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"

Triangle::Triangle()
{
	e0 = nullptr;
	e1 = nullptr;
	e2 = nullptr;
}

Triangle::Triangle(Edge *e0, Edge *e1, Edge *e2) : Triangle()
{
	this->e0 = e0;
	this->e1 = e1;
	this->e2 = e2;
}

void Triangle::render(sf::RenderWindow * window)
{
	sf::Vertex line0[] =
	{
		sf::Vertex(e0->v1->position,e0->color1),
		sf::Vertex(e0->v2->position,e0->color2)
	};

	sf::Vertex line1[] =
	{
		sf::Vertex(e1->v1->position,e1->color1),
		sf::Vertex(e1->v2->position,e1->color2)
	};

	sf::Vertex line2[] =
	{
		sf::Vertex(e2->v1->position,e2->color1),
		sf::Vertex(e2->v2->position,e2->color2)
	};

	window->draw(line0, 2, sf::Lines);
	window->draw(line1, 2, sf::Lines);
	window->draw(line2, 2, sf::Lines);

}