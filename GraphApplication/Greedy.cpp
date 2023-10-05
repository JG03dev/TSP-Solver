#include "pch.h"
#include "Graph.h"
#include <list>
// SalesmanTrackGreedy =========================================================

CTrack SalesmanTrackGreedy(CGraph& graph, CVisits &visits)
{
	if (graph.m_Vertices.empty() || graph.m_Edges.empty())
		return CTrack(&graph);
	
	list<CVertex*> candidats;
	list<CEdge*> camino;

	for (CVertex* v : visits.m_Vertices)
		if (v != visits.m_Vertices.front() && v != visits.m_Vertices.back())
			candidats.push_back(v);

	if (candidats.empty()) //No hi han candidats
	{
		for (CEdge* edge : visits.m_Vertices.front()->m_Edges)
		{
			if (edge->m_pDestination == visits.m_Vertices.back())
			{
				camino.push_back(edge);
				break;
			}
		}
	}
	
	CVertex* vActual = visits.m_Vertices.front();
	CVertex* vNext = NULL;

	while (!candidats.empty())
	{
		DijkstraQueue(graph, vActual);

		vNext = NULL;
		double min = numeric_limits<double>::max();

		for (CVertex* v : candidats)
		{
			if (v->m_DijkstraDistance < min)
			{
				min = v->m_DijkstraDistance;
				vNext = v;
			}
		}

		for (CEdge* e : vActual->m_Edges)
		{
			if (e->m_pDestination == vNext)
			{
				camino.push_back(e);
				break;
			}
		}

		candidats.remove(vNext);
		vActual = vNext;
	}

	CTrack track(&graph);
	for (CEdge* e : camino)
		track.AddLast(e);

	return track;
}
