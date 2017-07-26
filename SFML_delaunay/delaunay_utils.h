#pragma once

#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <vector>

#include <cmath>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#define CUSTOM_RED 255,46,109
#define CUSTOM_BLACK 15,15,15
#define CUSTOM_GREEN 35,220,35
#define CUSTOM_WHITE_ALPHA 255,255,255,100

#define VERTEX_SNAP_DISTANCE 15.0f

#define VERTEX_COUNT 50

class IRenderable;
class vertex;
class edge;
class circumcircle;
class utils;

// rozne narzedzia pomocnicze
class utils
{
public:
	// 1. misc
	static void msgbox(std::string wiadomosc)
	{
		MessageBoxA(NULL, wiadomosc.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
	}

	// 2. operacje wektorowe

	// sprawdza czy punkt cp i p naleza do tej samej polplaszczyzny
	// wyznaczanej przez krawedz f oraz punkt p.
	static bool same_halfspace_test(edge* f, vertex* p, vertex* cp);

	static inline float vector_magnitude(sf::Vector2f *vec)
	{
		return sqrtf(vec->x*vec->x + vec->y*vec->y);
	}

	// normalizacja
	static inline sf::Vector2f vector_normalized(sf::Vector2f *original)
	{
		float length = utils::vector_magnitude(original);
		sf::Vector2f normalized(original->x / length, original->y / length);
		return normalized;
	}

	static inline void vector_normalize(sf::Vector2f *modified)
	{
		float length = utils::vector_magnitude(modified);
		modified->x /= length;
		modified->y /= length;
	}

	// iloczyn skalarny wektorow
	static inline float vector_dotproduct(sf::Vector2f *v1, sf::Vector2f *v2)
	{
		return (v1->x*v2->x) + (v1->y*v2->y);
	}
	
	// kat pomiedzy wektorami; rezultat w radianach
	static inline float vector_anglebetween(sf::Vector2f *v1, sf::Vector2f *v2)
	{
		sf::Vector2f v1_n = utils::vector_normalized(v1);
		sf::Vector2f v2_n = utils::vector_normalized(v2);
		return acosf(utils::vector_dotproduct(&v1_n, &v2_n));
	}

	// szybki opis wektora
	static inline std::string vector_description(sf::Vector2f * vec, char * description)
	{
		std::string result = std::string(description) +
		std::string(": [x:") + std::to_string(vec->x) +
		std::string(" y:") + std::to_string(vec->y) +
		std::string("]");

		return result;
	}
	static inline std::string vector_description(sf::Vector2i * vec, char * description)
	{
		std::string result = std::string(description) +
			std::string(": [x:") + std::to_string(vec->x) +
			std::string(" y:") + std::to_string(vec->y) +
			std::string("]");

		return result;
	}
	static inline std::string vector_description(sf::Vector2u * vec, char * description)
	{
		std::string result = std::string(description) +
			std::string(": [x:") + std::to_string(vec->x) +
			std::string(" y:") + std::to_string(vec->y) +
			std::string("]");

		return result;
	}
	
	// 3. algorytmy
	static std::vector<edge*> convex_hull(std::vector<vertex*> &pointset);
	static void evaluate_triangle(int i, int j, int k, std::vector<vertex*>& pointset);
	static std::vector<edge*> dt_bruteforce(std::vector<vertex*> &pointset);
	static sf::Vector2f circumcenter(sf::Vector2f& a, sf::Vector2f& b, sf::Vector2f& c);

};

// interfejs obiektow widocznych
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
	void setVisible(bool _visible) { visible = _visible; }
	virtual void render(sf::RenderWindow *window) = 0;
};

class vertex : public IRenderable
{
public:
	static std::vector<vertex*> vertices;

	sf::Vector2f position;
	sf::CircleShape shape;
	sf::Color fillColor;
	sf::Color outlineColor;

	vertex();	
	vertex(float, float);

	inline void moveTo(float, float);
	inline void moveTo(sf::Vector2f);
	void moveBy(float, float);

	virtual void render(sf::RenderWindow *window);
};

class edge : public IRenderable
{
public:
	vertex *v1, *v2;

	sf::Color color1;
	sf::Color color2;

	edge();
	edge(vertex *v1, vertex *v2);

	void setColor1(sf::Color);
	void setColor2(sf::Color);
	void setColor(sf::Color);
	virtual void render(sf::RenderWindow *window);
};

class circumcircle : public IRenderable
{
public:
	static int redColorProgression;

	sf::CircleShape shape;
	sf::Color fillColor;
	sf::Color outlineColor;

	sf::Vector2f center;
	float radius;

	circumcircle();
	circumcircle(sf::Vector2f center, float radius);
	virtual void render(sf::RenderWindow *window);
};