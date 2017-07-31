#include "Vertex.h"

std::vector<Vertex*> Vertex::vertices;

Vertex::Vertex()
{
	Vertex::vertices.push_back(this);
}
Vertex::Vertex(float x, float y) : Vertex()
{
	position.x = x;
	position.y = y;

	fillColor = sf::Color::Transparent;
	outlineColor = sf::Color(CUSTOM_RED);

	shape.setOutlineThickness(1.0f);

	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);

	this->shape.setRadius(1.0f);
	this->shape.setPosition(position);
}

inline void Vertex::moveTo(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
}

inline void Vertex::moveTo(sf::Vector2f newPos)
{
	this->position = newPos;
}
void Vertex::moveBy(float x, float y)
{
	this->position.x += x;
	this->position.y += y;
}
/*
void Vertex::render(sf::RenderWindow *window)
{
	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);
	sf::Vector2f offset(shape.getRadius(), shape.getRadius());
	this->shape.setPosition(position - offset);
	window->draw(shape);
}
*/