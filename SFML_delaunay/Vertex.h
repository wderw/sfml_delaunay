#pragma once

#include "common.h"
#include "IRenderable.h"

class Vertex// : public IRenderable
{
public:
	static std::vector<Vertex*> vertices;

	sf::Vector2f position;
	sf::CircleShape shape;
	sf::Color fillColor;
	sf::Color outlineColor;

	Vertex();
	Vertex(float, float);

	inline void moveTo(float, float);
	inline void moveTo(sf::Vector2f);
	void moveBy(float, float);

	//virtual void render(sf::RenderWindow *window);
};