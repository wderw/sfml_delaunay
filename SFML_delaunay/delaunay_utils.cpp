#include "delaunay_utils.h"

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

void edge::setOrigin(sf::Vector2f* origin)
{
	simplex_origin = origin;
	circumcircle* c = new circumcircle(*simplex_origin, 1.0f);
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

bool utils::same_halfspace_test(edge * f, vertex * p, sf::Vector2f &cp)
{
	sf::Vector2f v(f->v2->position.x - f->v1->position.x, f->v2->position.y - f->v1->position.y);
	sf::Vector2f w(v.y, -v.x);

	float result_p = utils::vector_dotproduct(&(p->position - f->v1->position), &w);
	float result_cp = utils::vector_dotproduct(&(cp - f->v1->position), &w);

	if (result_p * result_cp < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

sf::Vector2f utils::triangle_centerofmass(vertex * a, vertex * b, vertex * c)
{
	return sf::Vector2f((a->position.x + b->position.x + c->position.x) / 3,
		(a->position.y + b->position.y + c->position.y) / 3);
}
// patrz wikipedia: algorytm jarvisa
std::vector<edge*> utils::convex_hull(std::vector<vertex*> &pointset)
{
	std::vector<edge*> result;

	if (pointset.size() == 0)
	{
		return result;
	}

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
	//min->fillColor = sf::Color::Yellow;
	

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
	//best->fillColor = sf::Color::Cyan;	
	edge* e = new edge(current, best);
	result.push_back(e);
	current = best;
	minAngle = FLT_MAX;

	sf::Vector2f resultVector = previousVector;

	// wrap
	while (best != min)
	{
		//best->fillColor = sf::Color::Cyan;

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

void utils::make_simplex(edge * f, std::vector<vertex*>& pointset)
{
	float minDD = FLT_MAX;
	vertex* best = nullptr;

	for (int i = 0; i < pointset.size(); ++i)
	{
		vertex* p = pointset[i];

		if (p == f->v1 || p == f->v2)
		{
			continue;
		}

		if (utils::same_halfspace_test(f, p, *f->simplex_origin))
		{
			continue;
		}

		float currentDD = utils::delaunay_distance(f, p);

		if (currentDD < minDD)
		{
			minDD = currentDD;
			best = p;
		}
	}

	if (best != nullptr)
	{
		best->fillColor = sf::Color::Blue;

		edge* e1 = new edge(f->v1, best);
		edge* e2 = new edge(f->v2, best);

		sf::Vector2f origin(utils::triangle_centerofmass(f->v1, f->v2, best));
		e1->setOrigin(&origin);
		e2->setOrigin(&origin);
	}
}

void utils::dt_dewall(std::vector<vertex*>& pointset)
{
	float alfa = 500;
	sf::Vector2f pos1(alfa, 0);
	sf::Vector2f pos2(alfa, 900);
	line* cuttingLine = new line(sf::Vertex(pos1, sf::Color::Cyan), sf::Vertex(pos2, sf::Color::Cyan));

	std::vector<edge*> hull = utils::convex_hull(pointset);

	float minDist1 = FLT_MAX;
	float currentDistance;
	vertex* currentVertex = nullptr,*p1 = nullptr;

	// MakeFirstSimplex:
	for (int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		currentDistance = fabs(currentVertex->position.x - alfa);

		if (currentDistance < minDist1)
		{
			minDist1 = currentDistance;
			p1 = currentVertex;
		}
	}
	p1->fillColor = sf::Color::Green;

	bool rightSide;
	if (p1->position.x > alfa)
	{
		rightSide = true;
	}
	else
	{
		rightSide = false;
	}



	// znajdz p2 po drugiej stronie
	minDist1 = FLT_MAX;
	currentDistance = 0;
	currentVertex = nullptr;
	vertex* p2 = nullptr;

	for (int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		currentDistance = utils::vector_magnitude(&(currentVertex->position - p1->position));

		if (currentVertex->position.x - alfa < 0 && rightSide == false)
		{
			continue;
		}
		if (currentVertex->position.x - alfa >= 0 && rightSide == true)
		{
			continue;
		}

		if (currentDistance < minDist1)
		{
			minDist1 = currentDistance;
			p2 = currentVertex;
		}
	}
	p2->fillColor = sf::Color::Red;

	// znajdz p3
	float minRadius = FLT_MAX;
	float currentRadius;
	currentVertex = nullptr;
	currentDistance = 0;
	vertex* p3 = nullptr;
	sf::Vector2f circumcenter;

	for (int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		circumcenter = utils::circumcenter(p1->position, p2->position, currentVertex->position);
		currentRadius = utils::vector_magnitude(&sf::Vector2f(circumcenter.x - p1->position.x, circumcenter.y - p1->position.y));

		if (currentRadius < minRadius)
		{
			minRadius = currentRadius;
			p3 = currentVertex;
		}
	}
	p1->fillColor = sf::Color::Yellow;
	p2->fillColor = sf::Color::White;
	p3->fillColor = sf::Color::Cyan;

	utils::dt_bruteforce(pointset);

	

	/* generic simplex: */
	edge* f = new edge(p1, p2);
	edge* f2 = new edge(p2, p3);
	edge* f3 = new edge(p3, p1);

	f->setOrigin(&sf::Vector2f(utils::triangle_centerofmass(p1, p2, p3)));
	f2->setOrigin(&sf::Vector2f(utils::triangle_centerofmass(p1, p2, p3)));
	f3->setOrigin(&sf::Vector2f(utils::triangle_centerofmass(p1, p2, p3)));

	utils::make_simplex(f, pointset);
	utils::make_simplex(f2, pointset);
	utils::make_simplex(f3, pointset);

	/*
	float minDD = FLT_MAX;
	vertex* best = nullptr;

	for (int i = 0; i < pointset.size(); ++i)
	{
		vertex* p = pointset[i];

		if (p == p1 || p == p2)
		{
			continue;
		}

		if (utils::same_halfspace_test(f, p3, p))
		{
			continue;
		}

		float currentDD = utils::delaunay_distance(f, p);

		if (currentDD < minDD)
		{
			minDD = currentDD;
			best = p;
		}
	}

	if(best != nullptr)
	{ 
		best->fillColor = sf::Color::Blue;

		edge* e1 = new edge(p1, best);
		edge* e2 = new edge(p2, best);

		sf::Vector2f origin(utils::triangle_centerofmass(p1, p2, best));
		e1->setOrigin(&origin);
		e2->setOrigin(&origin);
	}
	*/
}

// funkcja delaunay distance (dd) zdefiniowana w publikacji algorytmu DeWall
float utils::delaunay_distance(edge * f, vertex * p)
{
	sf::Vector2f circumcenter = utils::circumcenter(f->v1->position, f->v2->position, p->position);

	float radius = utils::vector_magnitude(&sf::Vector2f(circumcenter.x - p->position.x, circumcenter.y - p->position.y));

	if (same_halfspace_test(f, p, circumcenter))
	{
		return radius;
	}
	else
	{
		return -radius;
	}
}

bool utils::contains_edge(std::vector<edge*>& edges, edge* f)
{
	for (int i = 0; i < edges.size(); ++i)
	{
		if ((edges[i]->v1 == f->v1 && edges[i]->v2 == f->v2) || (edges[i]->v2 == f->v1 && edges[i]->v1 == f->v2))
		{
			return true;
		}
	}
	return false;
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

	e0->setColor(sf::Color(255, 255, 255, 25));
	e1->setColor(sf::Color(255, 255, 255, 25));
	e2->setColor(sf::Color(255, 255, 255, 25));
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

line::line()
{
}

void line::render(sf::RenderWindow * window)
{
	window->draw(pos, 2, sf::Lines);
}

line::line(sf::Vertex& v1, sf::Vertex& v2)
{
	pos[0] = v1;
	pos[1] = v2;
}
