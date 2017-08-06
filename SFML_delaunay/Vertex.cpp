#include "Vertex.h"
#include "Utils.h"

std::vector<Vertex*> Vertex::vertices;

Vertex::Vertex()
{
	Vertex::vertices.push_back(this);
}
Vertex::Vertex(double x, double y) : Vertex()
{
	position.x = x;
	position.y = y;
}

inline void Vertex::moveTo(double x, double y)
{
	this->position.x = x;
	this->position.y = y;
}

inline void Vertex::moveTo(Vector newPos)
{
	this->position = newPos;
}
void Vertex::moveBy(double x, double y)
{
	this->position.x += x;
	this->position.y += y;
}
/*
void Vertex::render(sf::RenderWindow *window)
{
	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);
	sf::Vector2<double> offset(shape.getRadius(), shape.getRadius());
	this->shape.setPosition(position - offset);
	window->draw(shape);
}
*/