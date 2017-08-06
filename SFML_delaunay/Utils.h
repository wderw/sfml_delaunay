#pragma once

#include "common.h"
#include "IRenderable.h"
#include "Vector.h"

// rozne narzedzia pomocnicze
class Utils
{
public:
	// 1. misc
	static void msgbox(std::string wiadomosc)
	{
		MessageBoxA(NULL, wiadomosc.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
	}

	static sf::Vector2f to_sfvector(Vector& v)
	{
		return sf::Vector2f(static_cast<float>(v.x), static_cast<float>(v.y));
	}

	static int theEnd;
	static bool vertexComparatorX(Vertex* A, Vertex* B);
	static void sortByX(std::vector<Vertex*>& pointset);

	// 2. operacje wektorowe
	// sprawdza czy punkt cp i p naleza do tej samej polplaszczyzny
	// wyznaczanej przez krawedz f oraz punkt p.
	static int sameHalfspaceTest(Edge* f, Vertex* p, Vector& cp);
	static double sameHalfspaceTestCheck(Edge * f, Vertex * p, Vector &cp);

	static double vectorMagnitude(Vector *vec);

	// srodek ciezkosci trojkata
	static Vector triangleCenterOfMass(Vertex* a, Vertex* b, Vertex* c);

	// normalizacja
	static Vector vectorNormalized(Vector *original);
	static void vectorNormalize(Vector *modified);

	// iloczyn skalarny wektorow
	static double dotProduct(Vector *v1, Vector *v2);

	// kat pomiedzy wektorami; rezultat w radianach
	static double vectorAngleBetween(Vector *v1, Vector *v2);

	// szybki opis wektora
	static std::string vectorDescription(Vector * vec, char * description);
	static std::string vectorDescription(sf::Vector2f * vec, char * description);

	static bool containsEdge(std::vector<Edge*>& edges, Edge* f);
	static bool sameEdge(Edge* A, Edge* B);

	// 3. algorytmy	
	static bool isIntersected(Vertex* v1, Vertex* v2, double alfa);
	static void evaluateTriangle(int i, int j, int k, std::vector<Vertex*>& pointset);
	static Vector* circumCenter(Vector& a, Vector& b, Vector& c);
	static std::vector<Edge*> dt_bruteforce(std::vector<Vertex*> &pointset);
	static void dt_dewall(std::vector<Vertex*>& pointset, double alfa);
	static double delaunayDistance(Edge* f, Vertex* p);
	static std::vector<Edge*> convexHull(std::vector<Vertex*> &pointset);
	static Triangle* makeSimplex(Edge* f, std::vector<Vertex*>& pointset,double alfa);
	static void updateAFL(std::list<Edge*>& AFL, Edge*);
};
