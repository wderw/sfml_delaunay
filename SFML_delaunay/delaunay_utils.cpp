﻿#include "delaunay_utils.h"

std::vector<vertex*> vertex::vertices;
std::vector<IRenderable*> IRenderable::renderables;

int circumcircle::redColorProgression = 0;

vertex::vertex()
{
	vertex::vertices.push_back(this);
}
vertex::vertex(float x, float y) : vertex()
{
	position.x = x;
	position.y = y;

	fillColor = sf::Color::Transparent;
	outlineColor = sf::Color(CUSTOM_RED);

	shape.setOutlineThickness(2.0f);

	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);

	this->shape.setRadius(3.0f);
	this->shape.setPosition(position);
}

inline void vertex::moveTo(float _x, float _y)
{
	this->position.x = _x;
	this->position.y = _y;
}

inline void vertex::moveTo(sf::Vector2f newPos)
{
	this->position = newPos;
}

void vertex::moveBy(float _x, float _y)
{
	this->position.x += _x;
	this->position.y += _y;
}

void vertex::render(sf::RenderWindow *window)
{
	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);
	sf::Vector2f offset(shape.getRadius(), shape.getRadius());
	this->shape.setPosition(position - offset);
	window->draw(shape);
}

edge::edge()
{
	v1 = nullptr;
	v2 = nullptr;

	color1 = sf::Color(CUSTOM_WHITE_ALPHA);
	color2 = sf::Color(CUSTOM_WHITE_ALPHA);
}

edge::edge(vertex * v1, vertex * v2) : edge()
{
	this->v1 = v1;
	this->v2 = v2;
}

void edge::setColor1(sf::Color c)
{
	color1 = c;
}
void edge::setColor2(sf::Color c)
{
	color2 = c;
}
void edge::setColor(sf::Color c)
{
	color1 = c;
	color2 = c;
}

void edge::render(sf::RenderWindow * window)
{
	sf::Vertex line[] =
	{
		sf::Vertex(v1->position,color1),
		sf::Vertex(v2->position,color2)
	};

	window->draw(line, 2, sf::Lines);
}

// funkcja sprawdza czy punkty p oraz cp leza po tej samej stronie linii
// wyznaczanej przez wektor f
// przypadek w ktorym punkt cp lub p leza na linii(f) moze powodowac problemy
// todo: funkcja powinna zwracac inna wartosc niz false/true np. (1,0,-1) w zaleznosci od znaku iloczynu iloczynow skalarnych.
bool utils::same_halfspace_test(edge * f, vertex * p, vertex * cp)
{
	sf::Vector2f v(f->v2->position.x - f->v1->position.x, f->v2->position.y - f->v1->position.y);
	sf::Vector2f w(v.y, -v.x);

	float result_p = utils::vector_dotproduct(&(p->position - f->v1->position), &w);
	float result_cp = utils::vector_dotproduct(&(cp->position - f->v1->position), &w);

	if (result_p * result_cp < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// patrz wikipedia: algorytm jarvisa
std::vector<edge*> utils::convex_hull(std::vector<vertex*> &pointset)
{
	std::vector<edge*> result;

	// find vertex with minimal y
	float ymin;

	vertex* current = pointset[0];
	vertex* min = nullptr;
	ymin = current->position.y;

	for (int i = 0; i < pointset.size(); i++)
	{
		current = pointset[i];
		if (current->position.y < ymin)
		{
			ymin = current->position.y;
			min = current;			
		}
	}
	// highlight it
	min->fillColor = sf::Color::Yellow;
	

	current = min;
	vertex* candidate = nullptr;
	vertex* best = nullptr;	
	sf::Vector2f u(1, 0); // wektor jednostkowy poczatkowy (tylko dla najwyzszego wierzcholka)
	float minAngle = FLT_MAX; // fltmax jest w tym przypadku bezpieczny i szybszy niz inicjalizacja
	sf::Vector2f previousVector;

	// pierwsza krawedz	
	for (int i = 0; i < pointset.size(); i++)
	{
		candidate = pointset[i];
		if (candidate == current) continue;

		sf::Vector2f v(candidate->position.x - current->position.x, candidate->position.y - current->position.y);
		

		float angle = utils::vector_anglebetween(&u, &v);
		if (angle < minAngle)
		{
			minAngle = angle;
			best = candidate;
			previousVector = v;
		}
	}	
	best->fillColor = sf::Color::Cyan;	
	edge* e = new edge(current, best);
	result.push_back(e);
	current = best;
	minAngle = FLT_MAX;

	sf::Vector2f resultVector = previousVector;

	// wrap
	while (best != min)
	{
		best->fillColor = sf::Color::Cyan;

		for (int i = 0; i < pointset.size(); i++)
		{
			candidate = pointset[i];
			if (candidate == current) continue;

			sf::Vector2f v(candidate->position.x - current->position.x, candidate->position.y - current->position.y);

			float angle = utils::vector_anglebetween(&resultVector, &v);
			if (angle < minAngle)
			{
				minAngle = angle;
				best = candidate;
				previousVector = v;
			}
		}
		
		e = new edge(current, best);
		result.push_back(e);
		current = best;
		minAngle = FLT_MAX;
		resultVector = previousVector;
	}
	return result;
}

void  utils::evaluate_triangle(int i, int j, int k, std::vector<vertex*>& pointset)
{
	// Indeks 'w' oznacza aktualnie sprawdzany wierzchołek.
	// Sprawdzamy czy znajduje się on wewnątrz koła opisanego na i,j,k.
	for (int w = 0; w < pointset.size(); w++)
	{
		// jeżeli aktualnie sprawdzany wierzchołek jest jednym z wierzchołków trójkąta
		// wtedy szukamy dalej.
		if (w == i) continue;
		if (w == j) continue;
		if (w == k) continue;

		// Funkcja Circumcenter
		// zwraca środek okręgu opisanego na trójkącie ijk i wrzuca rezultat do zmiennej
		// circumcenter.
		sf::Vector2f circumcenter = utils::circumcenter(pointset[i]->position, pointset[j]->position, pointset[k]->position);

		// policz promień koła opisanego na trójkącie ijk.
		float diffX = circumcenter.x - pointset[i]->position.x;
		float diffY = circumcenter.y - pointset[i]->position.y;
		//float distance = (float)Math.Sqrt(diffX * diffX + diffY * diffY);
		float distance = utils::vector_magnitude(&sf::Vector2f(diffX, diffY));

		// policz odległość wierzchołka sprawdzanego 'w' do środka okręgu.
		float wDiffX = circumcenter.x - pointset[w]->position.x;
		float wDiffY = circumcenter.y - pointset[w]->position.y;
		float wDistance = utils::vector_magnitude(&sf::Vector2f(wDiffX, wDiffY));

		// dodaj kółko do listy w razie gdybyśmy chcieli je obejrzeć na ekranie. (Nieistotne)
		// circumcircle* c = new circumcircle(circumcenter, distance);
		
		// jeżeli odległość punktu sprawdzanego 'w' od środka koła opisanego jest mniejsza niż
		// promień koła, to odrzuć trójkąt.
		if (wDistance < distance)
		{
			// odrzuć trójkąt
			return;
		}
	}
	edge* e0 = new edge(pointset[i], pointset[j]);
	edge* e1 = new edge(pointset[j], pointset[k]);
	edge* e2 = new edge(pointset[k], pointset[i]);
}


std::vector<edge*> utils::dt_bruteforce(std::vector<vertex*>& pointset)
{
	for (int i = 0; i < pointset.size() - 2; ++i)
	{
		for (int j = i + 1; j < pointset.size() - 1; ++j)
		{
			for (int k = j + 1; k < pointset.size(); ++k)
			{
				utils::evaluate_triangle(i, j, k, pointset);
			}
		}
	}
	return std::vector<edge*>();
}

// srodek kola opisanego na trojkacie abc
sf::Vector2f utils::circumcenter(sf::Vector2f& a, sf::Vector2f& b, sf::Vector2f& c)
{
	float x1 = a.x;
	float y1 = a.y;

	float x2 = b.x;
	float y2 = b.y;

	float x3 = c.x;
	float y3 = c.y;

	float A, B, C, D, E, F, G;

	A = x2 - x1;
	B = y2 - y1;
	C = x3 - x1;
	D = y3 - y1;
	E = A * (x1 + x2) + B * (y1 + y2);
	F = C * (x1 + x3) + D * (y1 + y3);
	G = 2.0f * (A * (y3 - y2) - B * (x3 - x2));
	if (G == 0)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		float Px = (D * E - B * F) / G;
		float Py = (A * F - C * E) / G;

		// tutaj zwraca wartość jako punkt.
		return sf::Vector2f(Px, Py);
	}
}

circumcircle::circumcircle()
{
	center = sf::Vector2f(0.0f, 0.0f);
	radius = 0.0f;
	if (!(circumcircle::redColorProgression + 35 > 255))
	{
		circumcircle::redColorProgression += 35;
	}	
}

circumcircle::circumcircle(sf::Vector2f center, float radius) : circumcircle()
{
	this->center = center;
	this->radius = radius;

	fillColor = sf::Color::Transparent;
	outlineColor = sf::Color(circumcircle::redColorProgression, 225, 120);

	shape.setOutlineThickness(2.0f);

	this->shape.setOutlineColor(outlineColor);
	this->shape.setFillColor(fillColor);

	this->shape.setRadius(radius);
	this->shape.setPosition(center);

	this->shape.setPointCount(100);
}

void circumcircle::render(sf::RenderWindow * window)
{
	//this->shape.setOutlineColor(outlineColor);
	//this->shape.setFillColor(fillColor);
	sf::Vector2f offset(shape.getRadius(), shape.getRadius());
	this->shape.setPosition(center - offset);
	window->draw(shape);
}
