#include "Line.h"

Line::Line()
{
}

void Line::render(sf::RenderWindow * window)
{
	window->draw(pos, 2, sf::Lines);
}

Line::Line(sf::Vertex& v1, sf::Vertex& v2)
{
	pos[0] = v1;
	pos[1] = v2;
}