#pragma once

#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>

#include <cmath>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define CUSTOM_RED 255,46,109
#define CUSTOM_BLACK 15,15,15
#define CUSTOM_GREEN 35,220,35
#define CUSTOM_WHITE_ALPHA 255,255,255,220

#define VERTEX_SNAP_DISTANCE 15.0
#define FIRST_SIMPLEX_OFFSET_EPSILON 0.0000000000001


#define VERTEX_COUNT 1000
#define ITER_COUNT 1300

//706 iteracja = odbicie od dolnej sciany

// narzedzia
class Utils;

// interfejs obiektow widocznych
class IRenderable;

// klasy implementujace renderable:
class Vertex;
class Edge;
class Circle;
class Line;
class Triangle;
class Vector;
