#include "pch.h"
#include "Graph.h"
#include <queue>
#include <list>
// SalesmanTrackGreedy =========================================================

CTrack SalesmanTrackGreedy(CGraph& graph, CVisits &visits)
{
	if (graph.m_Vertices.empty() || graph.m_Edges.empty())
		return CTrack(&graph);
	
	struct comparator {
		bool operator()(CVertex* pV1, CVertex* pV2) {
			return pV1->m_DijkstraDistance > pV2->m_DijkstraDistance;
		}
	};
	priority_queue<CVertex*, std::vector<CVertex*>, comparator> candidats;

	for (CVertex* v : visits.m_Vertices)
		if (v != visits.m_Vertices.front() && v != visits.m_Vertices.back())
			candidats.push(v);
	

	CVertex* vActual = visits.m_Vertices.front();
	CVertex* vNext = NULL;

	list<CEdge*> camino;

	while (!candidats.empty())
	{
		Dijkstra(graph, vActual);
		vNext = candidats.top();

		/*DEBO BUSCAR ENTRE LOS MAS PEQUEÑOS A PARTIR DE VACTUAL, BORRAR BUCLE DE ABAJO Y QUITAR PRIORITY QUEUE (creo)*/

		for (CEdge* edge : vActual->m_Edges)
		{
			if (edge->m_pDestination == vNext)
			{
				camino.push_back(edge);
				break;
			}
		}

		candidats.pop();
		vActual = vNext;
	}

	CTrack track(&graph);
	for (CEdge* edge : camino)
	{
		track.AddLast(edge);
	}

	return track;
}
