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

	static bool vertexComparatorX(Vertex* A, Vertex* B);

	static void sortByX(std::vector<Vertex*>& pointset);


	// 2. operacje wektorowe

	// sprawdza czy punkt cp i p naleza do tej samej polplaszczyzny
	// wyznaczanej przez krawedz f oraz punkt p.
	static int sameHalfspaceTest(Edge* f, Vertex* p, sf::Vector2f& cp);

	static float vectorMagnitude(sf::Vector2f *vec);


	// srodek ciezkosci trojkata
	static sf::Vector2f triangleCenterOfMass(Vertex* a, Vertex* b, Vertex* c);


	// normalizacja
	static sf::Vector2f vectorNormalized(sf::Vector2f *original);

	static void vectorNormalize(sf::Vector2f *modified);


	// iloczyn skalarny wektorow
	static float dotProduct(sf::Vector2f *v1, sf::Vector2f *v2);


	// kat pomiedzy wektorami; rezultat w radianach
	static float vectorAngleBetween(sf::Vector2f *v1, sf::Vector2f *v2);


	// szybki opis wektora
	static std::string vectorDescription(sf::Vector2f * vec, char * description);

	static bool containsEdge(std::vector<Edge*>& edges, Edge* f);
	static bool sameEdge(Edge* A, Edge* B);


	// 3. algorytmy	
	static bool isIntersected(Edge* e, float alfa);
	static void evaluateTriangle(int i, int j, int k, std::vector<Vertex*>& pointset);
	static sf::Vector2f circumCenter(sf::Vector2f& a, sf::Vector2f& b, sf::Vector2f& c);
	static std::vector<Edge*> dt_bruteforce(std::vector<Vertex*> &pointset);
	static void dt_dewall(std::vector<Vertex*>& pointset);
	static float delaunayDistance(Edge* f, Vertex* p);
	static std::vector<Edge*> convexHull(std::vector<Vertex*> &pointset);
	static Triangle* makeSimplex(Edge* f, std::vector<Vertex*>& pointset,float alfa);
	static void updateAFL(std::list<Edge*>& AFL, Edge*);

};