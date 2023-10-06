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
	//Inicialitzar les distàncies dels vèrtexs a infinit excepte la del vèrtex pStart que serà 0 (double m_DijkstraDistance).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraDistance = numeric_limits<double>::max();
	pStart->m_DijkstraDistance = 0;
	//Marcar tots el vèrtex com no visitats (bool m_DijkstraVisit).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraVisit = false;
	//pActual serà el vèrtex actual que l’inicialitzarem amb el vèrtex pStart
	CVertex* pActual = pStart;

	//Repetir mentre pActual!=NULL
	while (pActual != NULL)
	{
		//Recorre tots els veïns v de pActual i fer el següent
		for (CEdge* vei : pActual->m_Edges){
			/*Si la distància de v és més grossa que la distància del vèrtex actual més la longitud de l’aresta
				que els uneix, actualitzar la distancia de v amb la distància del vèrtex actual més la longitud
				de l’aresta que els uneix*/
			if (pActual->m_DijkstraDistance + vei->m_Length < vei->m_pDestination->m_DijkstraDistance){
				vei->m_pDestination->m_DijkstraDistance = vei->m_Length + pActual->m_DijkstraDistance;
				vei->m_pDestination->m_pDijkstraPrevious = vei;
			}
		}

		// Marcar pActual com visitat
		pActual->m_DijkstraVisit = true;

		//pActual=vèrtex no visitat amb distancia més petita o NULL si no hi ha vèrtexs no visitats
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
		bool operator()(pair<double, CVertex*> pE1, pair<double, CVertex*> pE2) {
			return pE1.first > pE2.first;
		}
	};
	
	priority_queue<pair<double, CVertex*>, std::vector<pair<double, CVertex*>>, comparator> queue;

	//Inicialitzar les distàncies dels vèrtexs a infinit excepte la del vèrtex pStart que serà 0 (double m_DijkstraDistance).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraDistance = numeric_limits<double>::max();
	pStart->m_DijkstraDistance = 0;
	double newDist;
	//Marcar tots el vèrtex com no visitats (bool m_DijkstraVisit).
	for (CVertex& v : graph.m_Vertices) v.m_DijkstraVisit = false;

	//pActual serà el vèrtex actual que l’inicialitzarem amb el vèrtex pStart
	queue.push(pair<double, CVertex*>(0.0, pStart));

	//Repetir mentre pActual!=NULL
	while (!queue.empty())
	{
		CVertex* va = queue.top().second;
		double distVa = queue.top().first;
		queue.pop();

		//Recorre tots els veïns v de pActual i fer el següent
		for (CEdge* vei : va->m_Edges)
		{
			if (vei->m_pDestination->m_DijkstraVisit)
				continue;
			/*Si la distància de v és més grossa que la distància del vèrtex actual més la longitud de l’aresta
				que els uneix, actualitzar la distancia de v amb la distància del vèrtex actual més la longitud
				de l’aresta que els uneix*/
			if (distVa + vei->m_Length < vei->m_pDestination->m_DijkstraDistance)
			{
				newDist = vei->m_Length + distVa;
				vei->m_pDestination->m_DijkstraDistance = newDist;
				vei->m_pDestination->m_pDijkstraPrevious = vei;
				queue.push(pair<double, CVertex*>(newDist, vei->m_pDestination));
			}
		}
		
		// Marcar pActual com visitat
		va->m_DijkstraVisit = true;
	}
}
