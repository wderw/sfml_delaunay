#include "common.h"
#include "Utils.h"
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
		sf::Vertex(Utils::ToVector2f(e0->v1->position),sf::Color::White),
		sf::Vertex(Utils::ToVector2f(e0->v2->position),sf::Color::White)
	};

	sf::Vertex line1[] =
	{
		sf::Vertex(Utils::ToVector2f(e1->v1->position),sf::Color::White),
		sf::Vertex(Utils::ToVector2f(e1->v2->position),sf::Color::White)
	};

	sf::Vertex line2[] =
	{
		sf::Vertex(Utils::ToVector2f(e2->v1->position),sf::Color::White),
		sf::Vertex(Utils::ToVector2f(e2->v2->position),sf::Color::White)
	};

	window->draw(line0, 2, sf::Lines);
	window->draw(line1, 2, sf::Lines);
	window->draw(line2, 2, sf::Lines);
}