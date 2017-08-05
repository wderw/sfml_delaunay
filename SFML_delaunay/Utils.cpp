#include "common.h"

#include "Utils.h"
#include "Circle.h"
#include "Vertex.h"
#include "Edge.h"
#include "Triangle.h"
#include "Line.h"

int Circle::redColorProgression = 0;
int Utils::theEnd = 0;


inline bool Utils::vertexComparatorX(Vertex * A, Vertex * B)
{
	if (A->position.x < B->position.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline void Utils::sortByX(std::vector<Vertex*>& pointset)
{
	std::sort(pointset.begin(), pointset.end(), vertexComparatorX);
}
// funkcja sprawdza czy punkty p oraz cp leza po tej samej stronie linii
// wyznaczanej przez wektor f
// przypadek w ktorym punkt cp lub p leza na linii(f) moze powodowac problemy
// todo: funkcja powinna zwracac inna wartosc niz false/true np. (1,0,-1) w zaleznosci od znaku iloczynu iloczynow skalarnych.
int Utils::sameHalfspaceTest(Edge * f, Vertex * p, sf::Vector2<double> &cp)
{
	sf::Vector2<double> v(f->v2->position.x - f->v1->position.x, f->v2->position.y - f->v1->position.y);
	sf::Vector2<double> w(v.y, -v.x);

	double result_p = Utils::dotProduct(&(p->position - f->v1->position), &w);
	double result_cp = Utils::dotProduct(&(cp - f->v1->position), &w);

	double result = result_p * result_cp;
	//return result;
	
	if (result < 0) // rozne strony linii
	{
		return -1;
	}
	else if (result > 0) // ta sama strona linii
	{
		return 1;
	}
	else // na linii
	{
		return 0;
	}
}

inline double Utils::sameHalfspaceTestCheck(Edge * f, Vertex * p, sf::Vector2<double> &cp)
{
	sf::Vector2<double> v(f->v2->position.x - f->v1->position.x, f->v2->position.y - f->v1->position.y);
	sf::Vector2<double> w(v.y, -v.x);

	double result_p = Utils::dotProduct(&(p->position - f->v1->position), &w);
	double result_cp = Utils::dotProduct(&(cp - f->v1->position), &w);

	double result = result_p * result_cp;
	return result;

}

inline double Utils::vectorMagnitude(sf::Vector2<double> * vec)
{
	return sqrt(vec->x*vec->x + vec->y*vec->y);
}

inline sf::Vector2<double> Utils::triangleCenterOfMass(Vertex * a, Vertex * b, Vertex * c)
{
	return sf::Vector2<double>((a->position.x + b->position.x + c->position.x) / 3,
		(a->position.y + b->position.y + c->position.y) / 3);
}
// patrz wikipedia: algorytm jarvisa
std::vector<Edge*> Utils::convexHull(std::vector<Vertex*> &pointset)
{
	if (pointset.size() < 3)
	{
		return std::vector<Edge*>();
	}

	std::vector<Edge*> result;

	if (pointset.size() == 0)
	{
		return result;
	}

	// find vertex with minimal y
	double ymin = DBL_MAX;

	Vertex* current = pointset[0];
	Vertex* min = nullptr;
	ymin = current->position.y;

	for (int i = 0; i < pointset.size(); ++i)
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
	Vertex* candidate = nullptr;
	Vertex* best = nullptr;
	sf::Vector2<double> u(1, 0); // wektor jednostkowy poczatkowy (tylko dla najwyzszego wierzcholka)
	double minAngle = DBL_MAX; // fltmax jest w tym przypadku bezpieczny i szybszy niz inicjalizacja
	sf::Vector2<double> previousVector;

	// pierwsza krawedz	
	for (int i = 0; i < pointset.size(); ++i)
	{
		candidate = pointset[i];
		if (candidate == current) continue;

		sf::Vector2<double> v(candidate->position.x - current->position.x, candidate->position.y - current->position.y);


		double angle = Utils::vectorAngleBetween(&u, &v);
		if (angle < minAngle)
		{
			minAngle = angle;
			best = candidate;
			previousVector = v;
		}
	}
	//best->fillColor = sf::Color::Cyan;	
	Edge* e = new Edge(current, best);
	result.push_back(e);
	current = best;
	minAngle = DBL_MAX;

	sf::Vector2<double> resultVector = previousVector;

	// wrap
	while (best != min)
	{
		//best->fillColor = sf::Color::Cyan;

		for (int i = 0; i < pointset.size(); ++i)
		{
			candidate = pointset[i];
			if (candidate == current) continue;

			sf::Vector2<double> v(candidate->position.x - current->position.x, candidate->position.y - current->position.y);

			double angle = Utils::vectorAngleBetween(&resultVector, &v);
			if (angle < minAngle)
			{
				minAngle = angle;
				best = candidate;
				previousVector = v;
			}
		}

		e = new Edge(current, best);
		result.push_back(e);
		current = best;
		minAngle = DBL_MAX;
		resultVector = previousVector;
	}
	return result;
}

Triangle* Utils::makeSimplex(Edge * f, std::vector<Vertex*>& pointset, double alfa)
{
	double minDD = DBL_MAX;
	//double minDD = 50;
	Vertex* best = nullptr;

	for (register int i = 0; i < pointset.size(); ++i)
	{
		Vertex* p = pointset[i];
		
		if (p == f->v1 || p == f->v2)
		{
			continue;
		}
		
		// pomin te ktore leza po zlej stronie f lub na f
		if (Utils::sameHalfspaceTest(f, p, f->simplex_origin) >= 0)
		{
			continue;
		}
		

		double currentDD = Utils::delaunayDistance(f, p);

		if ((currentDD < minDD))
		{
			minDD = currentDD;
			best = p;
		}
	}

	if (best != nullptr)
	{
		/* buduj tylko sciane
		if (!isIntersected(f->v1, best, alfa) && !isIntersected(f->v2,best, alfa))
		{
			return nullptr;
		}
		*/
		Edge *e1, *e2;
		e1 = new Edge(f->v1, best);
		e2 = new Edge(f->v2, best);
				
		sf::Vector2<double> origin(Utils::triangleCenterOfMass(f->v1, f->v2, best));
		e1->setOrigin(origin);
		e2->setOrigin(origin);		
		
		Triangle* t = new Triangle(e1, e2, f);

		return t;
	}
	else
	{
		return nullptr;
	}
}

struct special_compare : public std::unary_function<Edge*, bool>
{
	explicit special_compare(Edge* baseline) : baseline(baseline) {}

	bool operator() (Edge* arg)
	{
		return Utils::sameEdge(arg, baseline);
	}
	Edge* baseline;
};

// bezpieczna po wskazniku edge* e
inline void Utils::updateAFL(std::list<Edge*>& AFL, Edge* e)
{
	std::list<Edge*>::iterator it = std::find_if(AFL.begin(), AFL.end(), special_compare(e));
	if (it == AFL.end())
	{
		AFL.push_back(e);
	}
	else
	{
		AFL.remove(*it);
	}

	/* old crap
	for (auto const& i : AFL)
	{
		if (sameEdge(i, e))
		{
			AFL.remove(i);
			return;
		}
	}
	AFL.push_back(e);
	*/
}

void Utils::dt_dewall(std::vector<Vertex*>& pointset)
{
	if (pointset.size() < 3)
	{
		return;
	}

	Utils::sortByX(pointset);

	double alfa = pointset[(pointset.size() / 2)]->position.x + FIRST_SIMPLEX_OFFSET_EPSILON;

	sf::Vector2<double> pos1(alfa, 0);
	sf::Vector2<double> pos2(alfa, 900);
	Line* cuttingLine = new Line(sf::Vertex(sf::Vector2f(pos1), sf::Color::Cyan), sf::Vertex(sf::Vector2f(pos2), sf::Color::Cyan));

	//std::vector<Edge*> hull = Utils::convexHull(pointset);

	double minDist1 = DBL_MAX;
	double currentDistance;
	Vertex* currentVertex = nullptr, *p1 = nullptr;

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
	minDist1 = DBL_MAX;
	currentDistance = 0;
	currentVertex = nullptr;
	Vertex* p2 = nullptr;

	for (int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		currentDistance = Utils::vectorMagnitude(&(currentVertex->position - p1->position));

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
	double minRadius = DBL_MAX;
	double currentRadius;
	currentVertex = nullptr;
	currentDistance = 0;
	Vertex* p3 = nullptr;
	sf::Vector2<double>* circumcenter;

	for (int i = 0; i < pointset.size(); ++i)
	{
		currentVertex = pointset[i];
		circumcenter = Utils::circumCenter(p1->position, p2->position, currentVertex->position);
		if (circumcenter != nullptr)
		{
			currentRadius = Utils::vectorMagnitude(&sf::Vector2<double>(circumcenter->x - p1->position.x, circumcenter->y - p1->position.y));

			if (currentRadius < minRadius)
			{
				minRadius = currentRadius;
				p3 = currentVertex;
			}
		}
	}
	p1->fillColor = sf::Color::Yellow;
	p2->fillColor = sf::Color::White;
	p3->fillColor = sf::Color::Cyan;

	/* generic simplex: */
	Edge* f = new Edge(p1, p2);
	Edge* f2 = new Edge(p2, p3);
	Edge* f3 = new Edge(p3, p1);

	f->setOrigin(sf::Vector2<double>(Utils::triangleCenterOfMass(p1, p2, p3)));
	f2->setOrigin(sf::Vector2<double>(Utils::triangleCenterOfMass(p1, p2, p3)));
	f3->setOrigin(sf::Vector2<double>(Utils::triangleCenterOfMass(p1, p2, p3)));

	std::list<Edge*> AFL;


	Triangle* t0 = new Triangle(f, f2, f3);

	// oznacz pierwszy trojkat na czerwono
	//t0->e0->setColor(sf::Color::Red);
	//t0->e1->setColor(sf::Color::Red);
	//t0->e2->setColor(sf::Color::Red);

	updateAFL(AFL, f);
	updateAFL(AFL, f2);
	updateAFL(AFL, f3);

	// ogranicznik ilosci iteracji
	int counter = 0;
	while (!(AFL.empty())) // && counter < ITER_COUNT)
	{
		Edge* e = AFL.back();
		AFL.pop_back();

		Triangle* t = makeSimplex(e, pointset,alfa);

		if (t != nullptr)
		{
			
			updateAFL(AFL, t->e0);
			updateAFL(AFL, t->e1);		
		}
		counter++;
	}
}

// funkcja delaunay distance (dd) zdefiniowana w publikacji algorytmu DeWall
inline double Utils::delaunayDistance(Edge * f, Vertex * p)
{
	sf::Vector2<double>* circumcenter = Utils::circumCenter(f->v1->position, f->v2->position, p->position);

	if (circumcenter != nullptr)
	{
		double radius = Utils::vectorMagnitude(&sf::Vector2<double>(circumcenter->x - p->position.x, circumcenter->y - p->position.y));

		if (sameHalfspaceTest(f, p, *circumcenter) >= 0)
		{
			return radius;
		}
		else
		{
			return -radius;
		}
	}
	else return DBL_MAX;
}

inline sf::Vector2<double> Utils::vectorNormalized(sf::Vector2<double> * original)
{
	double length = Utils::vectorMagnitude(original);
	sf::Vector2<double> normalized(original->x / length, original->y / length);
	return normalized;
}

inline void Utils::vectorNormalize(sf::Vector2<double> * modified)
{
	double length = Utils::vectorMagnitude(modified);
	modified->x /= length;
	modified->y /= length;
}

inline double Utils::dotProduct(sf::Vector2<double> * v1, sf::Vector2<double> * v2)
{
	return (v1->x*v2->x) + (v1->y*v2->y);
}

inline double Utils::vectorAngleBetween(sf::Vector2<double> * v1, sf::Vector2<double> * v2)
{
	sf::Vector2<double> v1_n = Utils::vectorNormalized(v1);
	sf::Vector2<double> v2_n = Utils::vectorNormalized(v2);
	return acos(Utils::dotProduct(&v1_n, &v2_n));
}

std::string Utils::vectorDescription(sf::Vector2<double> * vec, char * description)
{
	std::string result = std::string(description) +
		std::string(": [x:") + std::to_string(vec->x) +
		std::string(" y:") + std::to_string(vec->y) +
		std::string("]");

	return result;
}

inline bool Utils::containsEdge(std::vector<Edge*>& edges, Edge* f)
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

inline bool Utils::sameEdge(Edge* A, Edge* B)
{
	if (A == B) return true;

	if ((A->v1 == B->v1 && A->v2 == B->v2) || (A->v1 == B->v2 && A->v2 == B->v1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool Utils::isIntersected(Vertex* v1, Vertex* v2, double alfa)
{
	if (v1->position.x < alfa && v2->position.x >= alfa)
	{
		return true;
	}
	else if (v2->position.x < alfa && v1->position.x >= alfa)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void  Utils::evaluateTriangle(int i, int j, int k, std::vector<Vertex*>& pointset)
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
		sf::Vector2<double>* circumcenter = Utils::circumCenter(pointset[i]->position, pointset[j]->position, pointset[k]->position);
		if (circumcenter == nullptr) return;

		// policz promień koła opisanego na trójkącie ijk.
		double diffX = circumcenter->x - pointset[i]->position.x;
		double diffY = circumcenter->y - pointset[i]->position.y;
		//double distance = (double)Math.Sqrt(diffX * diffX + diffY * diffY);
		double distance = Utils::vectorMagnitude(&sf::Vector2<double>(diffX, diffY));

		// policz odległość wierzchołka sprawdzanego 'w' do środka okręgu.
		double wDiffX = circumcenter->x - pointset[w]->position.x;
		double wDiffY = circumcenter->y - pointset[w]->position.y;
		double wDistance = Utils::vectorMagnitude(&sf::Vector2<double>(wDiffX, wDiffY));

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
	Edge* e0 = new Edge(pointset[i], pointset[j]);
	Edge* e1 = new Edge(pointset[j], pointset[k]);
	Edge* e2 = new Edge(pointset[k], pointset[i]);

	e0->setColor(sf::Color(255, 255, 255, 25));
	e1->setColor(sf::Color(255, 255, 255, 25));
	e2->setColor(sf::Color(255, 255, 255, 25));
}


std::vector<Edge*> Utils::dt_bruteforce(std::vector<Vertex*>& pointset)
{
	for (int i = 0; i < pointset.size() - 2; ++i)
	{
		for (int j = i + 1; j < pointset.size() - 1; ++j)
		{
			for (int k = j + 1; k < pointset.size(); ++k)
			{
				Utils::evaluateTriangle(i, j, k, pointset);
			}
		}
	}
	return std::vector<Edge*>();
}

// srodek kola opisanego na trojkacie abc
sf::Vector2<double>* Utils::circumCenter(sf::Vector2<double>& a, sf::Vector2<double>& b, sf::Vector2<double>& c)
{
	double x1 = a.x;
	double y1 = a.y;

	double x2 = b.x;
	double y2 = b.y;

	double x3 = c.x;
	double y3 = c.y;

	double A, B, C, D, E, F, G;

	A = x2 - x1;
	B = y2 - y1;
	C = x3 - x1;
	D = y3 - y1;
	E = A * (x1 + x2) + B * (y1 + y2);
	F = C * (x1 + x3) + D * (y1 + y3);
	G = 2.0f * (A * (y3 - y2) - B * (x3 - x2));
	if (G == 0)
	{
		//return sf::Vector2<double>(0, 0);
		return nullptr;
	}
	else
	{
		double Px = (D * E - B * F) / G;
		double Py = (A * F - C * E) / G;

		// tutaj zwraca wartość jako punkt.
		return new sf::Vector2<double>(Px, Py);
	}
}