#pragma once

#include "common.h"
#include "IRenderable.h"
#include "Vector.h"

// rozne narzedzia pomocnicze
class Utils
{
public:
	// 1. misc
	static void Msgbox(std::string wiadomosc)
	{
		MessageBoxA(NULL, wiadomosc.c_str(), "Info", MB_ICONINFORMATION | MB_OK);
	}

	static sf::Vector2f ToVector2f(Vector& v)
	{
		return sf::Vector2f(static_cast<float>(v.x), static_cast<float>(v.y));
	}

	static bool VertexComparatorX(Vertex* A, Vertex* B);
	static void SortByX(std::vector<Vertex*>& pointset);
	static bool VertexComparatorY(Vertex* A, Vertex* B);
	static void SortByY(std::vector<Vertex*>& pointset);

	// 2. operacje wektorowe
	// sprawdza czy punkt cp i p naleza do tej samej polplaszczyzny
	// wyznaczanej przez krawedz f oraz punkt p.
	static int SameHalfspaceTest(Edge* f, Vertex* p, Vector& cp);

	// srodek ciezkosci trojkata
	static Vector CenterOfMass(Vertex* a, Vertex* b, Vertex* c);

	// szybki opis wektora
	static std::string VectorDescription(Vector * vec, char * description);
	static std::string VectorDescription(sf::Vector2f * vec, char * description);

	static bool ContainsEdge(std::vector<Edge*>& edges, Edge* f);
	static bool SameEdge(Edge* A, Edge* B);

	// 3. algorytmy	
	static bool IsIntersected(Vertex* v1, Vertex* v2, double alfa);
	static void EvaluateTriangle(int i, int j, int k, std::vector<Vertex*>& pointset);
	static Vector* CircumCenter(Vector& a, Vector& b, Vector& c);
	static std::vector<Edge*> dt_bruteforce(std::vector<Vertex*> &pointset);
	static void dt_dewall(std::vector<Vertex*>& pointset, std::list<Edge*>& AFL, int);
	static double DelaunayDistance(Edge* f, Vertex* p);
	static std::vector<Edge*> ConvexHull(std::vector<Vertex*> &pointset);
	static Triangle* MakeSimplex(Edge* f, std::vector<Vertex*>& pointset,double alfa, bool (*IsIntersected)(Vertex*, Vertex*, double) );
	static void UpdateAFL(std::list<Edge*>& AFL, Edge*);

	static int WhichSideOfAlpha(Edge* f, double alfa);
	static int WhichSideOfAlphaY(Edge* f, double alfa);
	static void UpdateAFLBySide(std::list<Edge*>& AFL, std::list<Edge*>& AFL1, std::list<Edge*>& AFL2, Edge*, double alfa, int (*f)(Edge*, double));
	static Triangle* MakeFirstSimplex(std::vector<Vertex*>& pointset, double alfa);
	static void Utils::PointsetPartitionX(std::vector<Vertex*>& pointset, double alfa, std::vector<Vertex*>& P1, std::vector<Vertex*>& P2);
	static void Utils::PointsetPartitionY(std::vector<Vertex*>& pointset, double alfa, std::vector<Vertex*>& P1, std::vector<Vertex*>& P2);
	static bool IsIntersectedY(Vertex* v1, Vertex* v2, double alfa);
};

