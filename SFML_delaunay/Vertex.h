#pragma once

#include "common.h"
#include "IRenderable.h"
#include "Vector.h"

class Vertex// : public IRenderable
{
public:
	static std::vector<Vertex*> vertices;

	Vector position;

	Vertex();
	Vertex(double, double);

	void moveTo(double, double);
	void moveTo(Vector);
	void moveBy(double, double);

	//virtual void render(sf::RenderWindow *window);
};