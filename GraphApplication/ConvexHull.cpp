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

list<CVertex*> QuickHullRec(CVertex& A, CVertex& B, list<CVertex*>& Punts)
{
	if (Punts.empty())
	{
		return {&A, &B};
	}
	
	double max = 0.0;
	CVertex* C = nullptr;

	for (CVertex* v : Punts) {
		double area = AreaTriangle(A.m_Point, B.m_Point, v->m_Point);
		if (area > max)
		{
			C = v;
			max = area;
		}
	}

	if (C == nullptr)
	{
		return { &A, &B };
	}

	list<CVertex*> Left;
	list<CVertex*> Right;
	for (CVertex* v : Punts)
	{
		if (PosicioRespeteRecta(A.m_Point, C->m_Point, v->m_Point) > 0)
			Left.push_back(v);
		else
			if (PosicioRespeteRecta(C->m_Point, B.m_Point, v->m_Point) > 0)
				Right.push_back(v);
	}

	list<CVertex*> CHLeft = QuickHullRec(A, *C, Left);
	list<CVertex*> CHRight = QuickHullRec(*C, B, Right);

	for (CVertex* v : CHRight)
	{
		if (v != C) // No repetim C.
			CHLeft.push_back(v);
	}

	return CHLeft;
}

CConvexHull QuickHull(CGraph& graph)
{
	if (graph.m_Vertices.empty())
		return CConvexHull(&graph);

	CConvexHull CH(&graph);
	if (graph.m_Vertices.size() < 2) // 1 node o menys
	{
		CH.m_Vertices.push_back(&graph.m_Vertices.front());
		return CH;
	}

	CVertex* A = &graph.m_Vertices.front();
	CVertex* B = A;

	for (CVertex& v : graph.m_Vertices) {
		if (v.m_Point.m_X < A->m_Point.m_X || (v.m_Point.m_X == A->m_Point.m_X && v.m_Point.m_Y < A->m_Point.m_Y))
		{
			A = &v;
		}
		else if (v.m_Point.m_X > B->m_Point.m_X || (v.m_Point.m_X == B->m_Point.m_X && v.m_Point.m_Y > B->m_Point.m_Y))
		{
			B = &v;
		}
	}

	// Superposats.
	if (A == B)
	{
		CH.m_Vertices.push_back(A);
		return CH;
	}

	list<CVertex*> Up, Down;
	for (CVertex& v : graph.m_Vertices)
	{
		double pos = PosicioRespeteRecta(A->m_Point, B->m_Point, v.m_Point);
		if (pos > 0)
			Up.push_back(&v);
		else if (pos < 0)
			Down.push_back(&v);
	}

	list<CVertex*> CHUp = QuickHullRec(*A, *B, Up);
	list<CVertex*> CHDown = QuickHullRec(*B, *A, Down);

	for (CVertex* v : CHDown)
	{
		if (v != A && v != B) // No repetim A o B.
			CHUp.push_back(v);
	}

	CH.m_Vertices = CHUp;

	return CH;
}