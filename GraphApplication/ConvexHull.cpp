#include "pch.h"
#include "Graph.h"
#include "GraphApplicationDlg.h"
#include <set>



// =============================================================================
// CONVEX HULL =================================================================
// =============================================================================

// QuickHull ===================================================================

double PosicioRespeteRecta(CGPoint& a, CGPoint& b, CGPoint& c) {
	return (a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y);
}

double AreaTriangle(CGPoint& a, CGPoint& b, CGPoint c) {
	return abs((a.m_Y - b.m_Y) * (c.m_X - b.m_X) - (a.m_X - b.m_X) * (c.m_Y - b.m_Y)) / 2.0;
}

list<CVertex*> QuickHull(CVertex &A, CVertex &B, list<CVertex*>l)
{
	if (l.empty())
		return list<CVertex*>({ &A, &B });

	// Buscar major punt
	CVertex* C = nullptr;
	double Area = 0;
	for (auto c : l)
	{
		double NewA = AreaTriangle(A.m_Point, B.m_Point, c->m_Point);
		if (NewA >= Area)
		{
			C = c;
			Area = NewA;
		}
	}

	// Separar Conjunts
	list<CVertex*> left, right;
	for (auto d : l)
	{
		if (PosicioRespeteRecta(A.m_Point, C->m_Point, d->m_Point) < 0)
			left.emplace_back(d);
		else if (PosicioRespeteRecta(B.m_Point, C->m_Point, d->m_Point) > 0)
			right.emplace_back(d);
	}

	// Crides Recursives
	list<CVertex*>CHLeft = QuickHull(A, *C, left);
	list<CVertex*>CHRight = QuickHull(*C, B, right);

	// Ajuntar
	CHLeft.remove(C);
	CHLeft.splice(CHLeft.end(), CHRight, CHRight.begin(), CHRight.end());
	
	return CHLeft;
}

CConvexHull QuickHull(CGraph& graph)
{
	CConvexHull c = CConvexHull(&graph);

	// Casos especials
	if (graph.m_Vertices.empty()) // Graf buit
		return c;
	else if (graph.m_Vertices.size() == 1) // Graf d'un element
	{
		c.m_Vertices.emplace_back(&*graph.m_Vertices.begin());
		return c;
	}
	else if(graph.m_Vertices.size() == 2) // Graf de 2 elements
	{
		auto it = graph.m_Vertices.begin();
		CGPoint p(it->m_Point);
		c.m_Vertices.emplace_back(&*it); it++;
		if(p.m_X != it->m_Point.m_X || p.m_Y != it->m_Point.m_Y) //Cas superposats
			c.m_Vertices.emplace_back(&*it);
		return c;
	}

	// Buscar Extrems
	CVertex *A = nullptr, *B = nullptr; // A sera el punt mes dalt i B el punt mes baix
	double Highest = numeric_limits<double>::min(), Lowest = numeric_limits<double>::max();
	for (CVertex& d : graph.m_Vertices)
	{
		if (d.m_Point.m_Y > Highest)
		{
			Highest = d.m_Point.m_Y;
			A = &d;
		}
		if (d.m_Point.m_Y < Lowest)
		{
			Lowest = d.m_Point.m_Y;
			B = &d;
		}
	}
	// Separar Conjunts
	list<CVertex*> left, right;
	for (CVertex& d : graph.m_Vertices) 
	{
		if (PosicioRespeteRecta(A->m_Point, B->m_Point, d.m_Point) > 0)
			right.emplace_back(&d);
		else if (PosicioRespeteRecta(A->m_Point, B->m_Point, d.m_Point) < 0)
			left.emplace_back(&d);
	}

	// Crides Recursives
	c.m_Vertices = QuickHull(*A, *B, left);
	list<CVertex*>CHRight = QuickHull(*B, *A, right);

	// Ajuntar
	CHRight.remove(A);
	CHRight.remove(B);
	c.m_Vertices.splice(c.m_Vertices.end(), CHRight, CHRight.begin(), CHRight.end());

	return c;
}



