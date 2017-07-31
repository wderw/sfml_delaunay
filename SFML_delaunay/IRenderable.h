#pragma once

#include "common.h"

/* interfejs obiektow widocznych */
class IRenderable
{
protected:
	bool visible;

public:
	static std::vector<IRenderable*> renderables;

	IRenderable()
	{
		visible = true;
		IRenderable::renderables.push_back(this);
	}

	bool isVisible() { return visible; }
	void setVisible(bool visible) { this->visible = visible; }
	virtual void render(sf::RenderWindow *window) = 0;
};

