#pragma once

#include "common.h"
#include "IRenderable.h"

// rozne narzedzia pomocnicze
class Utils
{
public:
	// 1. misc
	static void msgbox(std::string wiadomosc)
	{
		MessageBoxA(NULL, wiadomosc.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
	}

	static int theEnd;
	static bool vertexComparatorX(Vertex* A, Vertex* B);

	static void sortByX(std::vector<Vertex*>& pointset);


	// 2. operacje wektorowe

	// sprawdza czy punkt cp i p naleza do tej samej polplaszczyzny
	// wyznaczanej przez krawedz f oraz punkt p.
	static int sameHalfspaceTest(Edge* f, Vertex* p, sf::Vector2<double>& cp);
	static double sameHalfspaceTestCheck(Edge * f, Vertex * p, sf::Vector2<double> &cp);

	static double vectorMagnitude(sf::Vector2<double> *vec);


	// srodek ciezkosci trojkata
	static sf::Vector2<double> triangleCenterOfMass(Vertex* a, Vertex* b, Vertex* c);


	// normalizacja
	static sf::Vector2<double> vectorNormalized(sf::Vector2<double> *original);

	static void vectorNormalize(sf::Vector2<double> *modified);


	// iloczyn skalarny wektorow
	static double dotProduct(sf::Vector2<double> *v1, sf::Vector2<double> *v2);


	// kat pomiedzy wektorami; rezultat w radianach
	static double vectorAngleBetween(sf::Vector2<double> *v1, sf::Vector2<double> *v2);


	// szybki opis wektora
	static std::string vectorDescription(sf::Vector2<double> * vec, char * description);

	static bool containsEdge(std::vector<Edge*>& edges, Edge* f);
	static bool sameEdge(Edge* A, Edge* B);


	// 3. algorytmy	
	static bool isIntersected(Vertex* v1, Vertex* v2, double alfa);
	static void evaluateTriangle(int i, int j, int k, std::vector<Vertex*>& pointset);
	static sf::Vector2<double>* circumCenter(sf::Vector2<double>& a, sf::Vector2<double>& b, sf::Vector2<double>& c);
	static std::vector<Edge*> dt_bruteforce(std::vector<Vertex*> &pointset);
	static void dt_dewall(std::vector<Vertex*>& pointset);
	static double delaunayDistance(Edge* f, Vertex* p);
	static std::vector<Edge*> convexHull(std::vector<Vertex*> &pointset);
	static Triangle* makeSimplex(Edge* f, std::vector<Vertex*>& pointset,double alfa);
	static void updateAFL(std::list<Edge*>& AFL, Edge*);

};