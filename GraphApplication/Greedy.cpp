#include "pch.h"
#include "Graph.h"
#include <stack>
// SalesmanTrackGreedy =========================================================

CTrack SalesmanTrackGreedy(CGraph& graph, CVisits& visits) {
	if (graph.m_Vertices.empty() || graph.m_Edges.empty())
		return CTrack(&graph);

	list<CVertex*> candidats;
	list<CEdge*> camino;

	for (CVertex* v : visits.m_Vertices)
		if (v != visits.m_Vertices.front() && v != visits.m_Vertices.back())
			candidats.push_back(v);

	CVertex* vActual = visits.m_Vertices.front();
	CVertex* vNext = NULL;

	while (!candidats.empty())
	{
		DijkstraQueue(graph, vActual);

		vNext = NULL;
		double min = numeric_limits<double>::max();

		// Trobar el seguent candidat
		for (CVertex* v : candidats)
		{
			if (v->m_DijkstraDistance < min)
			{
				min = v->m_DijkstraDistance;
				vNext = v;
			}
		}

		//Buscar cam� per anar fins al seguent candidat
		CVertex* aux = vNext;
		stack<CEdge*> camiVaV1; //Stack auxiliar per recolectar el cami de v a v1 (vActual a vNext)
		while (aux != vActual)
		{
			camiVaV1.push(aux->m_pDijkstraPrevious);
			aux = (aux->m_pDijkstraPrevious->m_pOrigin != aux) ? aux->m_pDijkstraPrevious->m_pOrigin : aux->m_pDijkstraPrevious->m_pDestination;
		}

		// Afegim el cami trobat en ordre invers
		while(!camiVaV1.empty())
		{
			camino.push_back(camiVaV1.top());
			camiVaV1.pop();
		}
		
		candidats.remove(vNext);
		vActual = vNext;
	}

	// Calcular camino hasta el ultimo nodo a partir de vActual
	DijkstraQueue(graph, vActual);

	vNext = visits.m_Vertices.back();
	double min = numeric_limits<double>::max();

	//Buscar cam� per anar fins al seguent candi
	CVertex* aux = vNext;
	stack<CEdge*> camiVaV1; //Stack auxiliar per recolectar el cami de v a v1 (vActual a vNext)
	while (aux != vActual)
	{
		camiVaV1.push(aux->m_pDijkstraPrevious);
		aux = (aux->m_pDijkstraPrevious->m_pOrigin != aux) ? aux->m_pDijkstraPrevious->m_pOrigin : aux->m_pDijkstraPrevious->m_pDestination;
	}

	// Afegim el cami trobat en ordre invers
	while (!camiVaV1.empty())
	{
		camino.push_back(camiVaV1.top());
		camiVaV1.pop();
	}

	CTrack track(&graph);
	for (CEdge* e : camino)
		track.AddLast(e);

	return track;
}
