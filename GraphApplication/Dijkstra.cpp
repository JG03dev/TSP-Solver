#include "pch.h"
#include "Graph.h"
#include <vector>
#include <queue>
#include <stack>

// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void Dijkstra(CGraph& graph, CVertex *pStart)
{
	//Inicialitzar les dist�ncies dels v�rtexs a infinit excepte la del v�rtex pStart que ser� 0 (double m_DijkstraDistance).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraDistance = numeric_limits<double>::max();
	pStart->m_DijkstraDistance = 0;
	//Marcar tots el v�rtex com no visitats (bool m_DijkstraVisit).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraVisit = false;
	//pActual ser� el v�rtex actual que l�inicialitzarem amb el v�rtex pStart
	CVertex* pActual = pStart;

	//Repetir mentre pActual!=NULL
	while (pActual != NULL)
	{
		//Recorre tots els ve�ns v de pActual i fer el seg�ent
		for (CEdge* vei : pActual->m_Edges){
			/*Si la dist�ncia de v �s m�s grossa que la dist�ncia del v�rtex actual m�s la longitud de l�aresta
				que els uneix, actualitzar la distancia de v amb la dist�ncia del v�rtex actual m�s la longitud
				de l�aresta que els uneix*/
			if (pActual->m_DijkstraDistance + vei->m_Length < vei->m_pDestination->m_DijkstraDistance){
				vei->m_pDestination->m_DijkstraDistance = vei->m_Length + pActual->m_DijkstraDistance;
				vei->m_pDestination->m_pDijkstraPrevious = vei;
			}
		}

		// Marcar pActual com visitat
		pActual->m_DijkstraVisit = true;

		//pActual=v�rtex no visitat amb distancia m�s petita o NULL si no hi ha v�rtexs no visitats
		pActual = NULL;
		double min_distance = numeric_limits<double>::max();
		for (CVertex& v : graph.m_Vertices)
		{
			if (!v.m_DijkstraVisit && v.m_DijkstraDistance < min_distance)
			{
				pActual = &v;
				min_distance = v.m_DijkstraDistance;
			}
		}
	}
}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void DijkstraQueue(CGraph& graph, CVertex *pStart)
{
	struct comparator {
		bool operator()(CVertex* pE1, CVertex* pE2) {
			return pE1->m_DijkstraDistance > pE2->m_DijkstraDistance;
		}
	};
	priority_queue<CVertex*, std::vector<CVertex*>, comparator> queue;

	//Inicialitzar les dist�ncies dels v�rtexs a infinit excepte la del v�rtex pStart que ser� 0 (double m_DijkstraDistance).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraDistance = numeric_limits<double>::max();
	pStart->m_DijkstraDistance = 0;
	//Marcar tots el v�rtex com no visitats (bool m_DijkstraVisit).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraVisit = false;

	//pActual ser� el v�rtex actual que l�inicialitzarem amb el v�rtex pStart
	queue.push(pStart);

	//Repetir mentre pActual!=NULL
	while (!queue.empty())
	{
		CVertex* va = queue.top();
		queue.pop();

		if (va->m_DijkstraVisit)
			continue;

		//Recorre tots els ve�ns v de pActual i fer el seg�ent
		for (CEdge* vei : va->m_Edges)
			/*Si la dist�ncia de v �s m�s grossa que la dist�ncia del v�rtex actual m�s la longitud de l�aresta
				que els uneix, actualitzar la distancia de v amb la dist�ncia del v�rtex actual m�s la longitud
				de l�aresta que els uneix*/
			if (va->m_DijkstraDistance + vei->m_Length < vei->m_pDestination->m_DijkstraDistance)
			{
				vei->m_pDestination->m_DijkstraDistance = vei->m_Length + va->m_DijkstraDistance;
				vei->m_pDestination->m_pDijkstraPrevious = vei;
				queue.push(vei->m_pDestination);
			}
		
		// Marcar pActual com visitat
		va->m_DijkstraVisit = true;
	}
}
