#pragma once

#include "common.h"
#include "IRenderable.h"
#include "Edge.h"

class Triangle : public IRenderable
{
public:
	Edge* e0;
	Edge* e1;
	Edge* e2;

	Triangle();
	Triangle(Edge*, Edge*, Edge*);

	virtual void render(sf::RenderWindow *window);
};