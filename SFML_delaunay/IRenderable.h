#pragma once

#include "common.h"
#include <mutex>
/* interfejs obiektow widocznych */
class IRenderable
{
protected:
	bool visible;
	

public:
	static std::mutex mutex;
	static std::vector<IRenderable*> renderables;

	IRenderable()
	{
		visible = true;
		mutex.lock();
		IRenderable::renderables.push_back(this);
		mutex.unlock();
	}

	bool isVisible() { return visible; }
	void setVisible(bool visible) { this->visible = visible; }
	virtual void render(sf::RenderWindow *window) = 0;
};

