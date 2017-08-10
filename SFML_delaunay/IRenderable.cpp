#include "IRenderable.h"

std::vector<IRenderable*> IRenderable::renderables;
std::mutex IRenderable::mutex;