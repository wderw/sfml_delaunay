#pragma once

#include "common.h"
#include "IRenderable.h"

class Line : public IRenderable
{
public:
	sf::Vertex pos[2];

	Line();
	Line(sf::Vertex& v1, sf::Vertex& v2);

	virtual void render(sf::RenderWindow *window);
};
