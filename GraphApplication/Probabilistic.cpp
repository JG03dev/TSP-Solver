#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 
#include <random>
#include <chrono>

// SalesmanTrackProbabilistic ==================================================
CTrack getDijkstraPrevio(CVertex* vo, CVertex* vd)
{
	CTrack camino = CTrack(NULL);
	CVertex* auxVertex = vd;
	while (auxVertex != vo)
	{
		camino.AddFirst(auxVertex->m_pDijkstraPrevious);
		if (auxVertex == auxVertex->m_pDijkstraPrevious->m_pDestination)
			auxVertex = auxVertex->m_pDijkstraPrevious->m_pOrigin;
		else
			auxVertex = auxVertex->m_pDijkstraPrevious->m_pDestination;
	}
	return camino;
}

CTrack SalesmanTrackProbabilistic(CGraph& graph, CVisits& visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
		return CTrack(&graph);

	// Hay que pasar por todos los que sean vertex to visit.
	for (CVertex& v : graph.m_Vertices) {
		v.m_JaHePassat = false;
	}

	CVertex* pInici = visits.m_Vertices.front();
	CVertex* vDesti = visits.m_Vertices.back();

	std::vector<std::vector<std::pair<CTrack, double>>> matrix_dijkstra;
	for (CVertex* vo : visits.m_Vertices) // Vertex Origen
	{
		DijkstraQueue(graph, vo);
		std::vector<std::pair<CTrack, double>> vector_dijkstra;
		for (CVertex* vd : visits.m_Vertices) // Vertex Desti
		{
			if (vo == vd || vo == pInici) {
				vector_dijkstra.emplace_back(std::make_pair(CTrack(NULL), 0.0));
			}
			else {
				CTrack caminoDijkstra = getDijkstraPrevio(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
			}
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}

	// CHANGE FROM HERE DOWN BELOW!
	int i = 0;
	/*
	priority_queue<CBBNodeAlumne*, std::vector<CBBNodeAlumne*>, comparator> queue;

	// Inicialization of the first node.
	queue.push(new CBBNodeAlumne(visits.GetNVertices(), 0));

	double maxLength = numeric_limits<double>::max();
	std::vector<int> optimum;
	while (!queue.empty()) {
		CBBNodeAlumne* pS = queue.top();
		queue.pop();

		int pV = pS->m_VisitesFetes[pS->m_numFets - 1]; // Ultim node del cami.
		if (pS->m_VisitesFetes.back() != -1) { // Desti arribat!
			// Solucion encontrada
			optimum = pS->m_VisitesFetes;
			delete pS;
			break;
		}
		else if (pS->m_VisitesFetes[visits.GetNVertices() - 2] != -1) // Cuando visitados todos.
		{
			double realDist = matrix_dijkstra[pV][pS->m_sizeVisits - 1].second;
			double w = pS->m_Length + realDist; //Distancia g (no hay heuristica)

			if (w < maxLength) {
				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, pS->m_sizeVisits - 1, w);

				maxLength = w; //Solucio parcial, actualitzem maxLength per podar
				queue.push(pSNew);
			}
		}
		else {
			for (int i = 0; i < pS->m_sizeVisits - 1; i++) {
				if (pS->m_VisitesAFer[i])  continue; //Recorrem nomes els que no hem visitat

				double realDist = matrix_dijkstra[pV][i].second;
				double w = pS->m_Length + realDist; //Distancia g (no hay heuristica)

				if (w < maxLength) {
					CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, i, w);
					if (pSNew->m_VisitesFetes.back() != -1) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
					queue.push(pSNew);
				}
			}
		}
		delete pS;
	}

	CTrack Res(&graph);
	for (int i = 1; i < optimum.size(); i++)
	{
		Res.Append(matrix_dijkstra[optimum[i - 1]][optimum[i]].first);
	}

	return Res;
	*/


	return CTrack(&graph);
}
