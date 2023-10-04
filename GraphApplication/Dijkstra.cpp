#include "pch.h"
#include "Graph.h"
#include <queue>
#include <stack>
#include <vector>
// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void Dijkstra(CGraph& graph, CVertex *pStart)
{

	for (CVertex& v : graph.m_Vertices)
	{
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
	}

	pStart->m_DijkstraDistance = 0;
	CVertex* pActual = pStart;

	while (pActual != NULL)
	{
		for (CEdge* e : pActual->m_Edges)
			if (e->m_pDestination->m_DijkstraDistance > pActual->m_DijkstraDistance + e->m_Length)
				e->m_pDestination->m_DijkstraDistance = pActual->m_DijkstraDistance + e->m_Length;
		
		pActual->m_DijkstraVisit = true;
		
		pActual = NULL;
		double min = numeric_limits<double>::max();
		for (CVertex& v : graph.m_Vertices)
			if (!v.m_DijkstraVisit && v.m_DijkstraDistance < min)
			{
				min = v.m_DijkstraDistance;
				pActual = &v;
			}		
	}
}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void DijkstraQueue(CGraph& graph, CVertex *pStart)
{
	struct comparator {
		bool operator()(CVertex* pV1, CVertex* pV2) {
			return pV1->m_DijkstraDistance > pV2->m_DijkstraDistance;
		}
	};
	priority_queue<CVertex*, std::vector<CVertex*>, comparator> queue;

	for (CVertex & v : graph.m_Vertices)
	{
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
	}
	pStart->m_DijkstraDistance = 0;
	queue.push(pStart);

	CVertex* va = NULL;

	while (!queue.empty())
	{
		va = queue.top();
		queue.pop();
		if (!va->m_DijkstraVisit)
		{
			for (CEdge* e : va->m_Edges)
			{
				if (e->m_pDestination->m_DijkstraDistance > va->m_DijkstraDistance + e->m_Length)
				{
					e->m_pDestination->m_DijkstraDistance = va->m_DijkstraDistance + e->m_Length;
					queue.push(e->m_pDestination);
				}
			}
			va->m_DijkstraVisit = true;
		}
	}	
}
