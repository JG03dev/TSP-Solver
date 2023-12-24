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
	CVertex* pInici = visits.m_Vertices.front();
	CVertex* vDesti = visits.m_Vertices.back();

	std::vector<std::vector<std::pair<CTrack, double>>> matrix_dijkstra;
	for (CVertex* vo : visits.m_Vertices) // Vertex Origen
	{
		DijkstraQueue(graph, vo);
		std::vector<std::pair<CTrack, double>> vector_dijkstra;
		for (CVertex* vd : visits.m_Vertices) // Vertex Desti
		{
			if (vo == vd) {
				vector_dijkstra.emplace_back(std::make_pair(CTrack(NULL), 0.0));
			}
			else {
				CTrack caminoDijkstra = getDijkstraPrevio(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
			}
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}

	std::vector<int> indexVisites(visits.GetNVertices());

	for (int i = 0; i < visits.GetNVertices(); ++i) {
		indexVisites[i] = i;
	}

	int total_tries = visits.GetNVertices() * 10'000;
	vector<int> CamiDefinitiu(visits.GetNVertices());
	double LongCamiMesCurt = numeric_limits<double>::max();



	for (int tries = 0; tries < total_tries; ++tries) {

		// Initialize random engine outside the loop
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 g(seed);

		// ------------------------- Inicialitzacio aleatoria -------------------------
		int origen = 0, desti = indexVisites.back();
		// Shuffle the elements in between
		std::shuffle(indexVisites.begin() + 1, indexVisites.end() - 1, g);
		// --------------------------------------------------------------------------- 

		double LongCamiActual = 0.0;
		for (int iV = 0; iV < indexVisites.size() - 1; ++iV) {
			LongCamiActual += matrix_dijkstra[indexVisites[iV]][indexVisites[iV + 1]].second;
		}

		if (LongCamiActual < LongCamiMesCurt) {
			LongCamiMesCurt = LongCamiActual;
			CamiDefinitiu = indexVisites;
		}

		vector<int> camiAux = indexVisites;

		for (int i = 1; i < visits.GetNVertices() - 2; ++i) {
			for (int j = i + 1; j < visits.GetNVertices() - 1; ++j) {

				LongCamiActual = 0.0;

				indexVisites = camiAux; // Tornar al indexVisits original del intent N.

				std::swap(indexVisites[i], indexVisites[j]); // Fem SWAP

				for (int iV = 0; iV < indexVisites.size() - 1; ++iV) {
					LongCamiActual += matrix_dijkstra[indexVisites[iV]][indexVisites[iV + 1]].second;
				}

				if (LongCamiActual < LongCamiMesCurt) {
					LongCamiMesCurt = LongCamiActual;
					CamiDefinitiu = indexVisites;
				}
			}
		}
	}

	CTrack shortestTrack(&graph);
	int auxOrigen = 0;
	for (int i : CamiDefinitiu)
	{
		if (i == 0)
			continue;
		shortestTrack.Append(matrix_dijkstra[auxOrigen][i].first);
		auxOrigen = i;
	}

	return shortestTrack;
}
