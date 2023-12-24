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

	int total_tries = visits.GetNVertices()*100000;
	vector<int> CamiDefinitiu;
	double distanciaDefinitiva = numeric_limits<double>::max();

	for (int tries = 0; tries < total_tries; tries++) {

		std::vector<int> indexVisites(visits.GetNVertices(), 0);

		for (int i = 0; i < visits.GetNVertices(); ++i) {
			indexVisites[i] = i;
		}

		double LongCamiActual = 0.0;
		double LongCamiMesCurt = (double)numeric_limits<float>::max();

		// ------------------------- Inicialitzacio aleatoria -------------------------
		int origen = 0, desti = indexVisites.back();
		// Shuffle the elements in between
		std::srand(std::time(0));
		std::random_shuffle(indexVisites.begin() + 1, indexVisites.end() - 1);
		// --------------------------------------------------------------------------- 

		for (int i = 1; i < indexVisites.size(); i++) {
			LongCamiActual += matrix_dijkstra[indexVisites[i]][indexVisites[i - 1]].second;
		}

		for (int i = 1; i < visits.GetNVertices() - 2; ++i) {
			for (int j = i + 1; j < visits.GetNVertices() - 1; ++j) {

				double cami_aux = LongCamiActual;

				if (indexVisites[i] != indexVisites[j] - 1 && indexVisites[i] != indexVisites[j] + 1) { // Nodes NOT next to each other.
					LongCamiActual = LongCamiActual - matrix_dijkstra[indexVisites[i] - 1][indexVisites[i]].second - matrix_dijkstra[indexVisites[i]][indexVisites[i] + 1].second -
						matrix_dijkstra[indexVisites[j] - 1][indexVisites[j]].second - matrix_dijkstra[indexVisites[j]][indexVisites[j] + 1].second + matrix_dijkstra[indexVisites[i] - 1][indexVisites[j]].second + matrix_dijkstra[indexVisites[j]][indexVisites[i] + 1].second
						+ matrix_dijkstra[indexVisites[j] - 1][indexVisites[i]].second + matrix_dijkstra[indexVisites[i]][indexVisites[j] + 1].second;
				}
				else { // Next to each other.
					LongCamiActual = LongCamiActual - matrix_dijkstra[indexVisites[j] - 2][indexVisites[j] - 1].second - matrix_dijkstra[indexVisites[j]][indexVisites[j] - 1].second
						+ matrix_dijkstra[indexVisites[j] - 2][indexVisites[j]].second + matrix_dijkstra[indexVisites[j] - 1][indexVisites[j] + 1].second;
				}

				if (LongCamiActual < LongCamiMesCurt) {
					LongCamiMesCurt = LongCamiActual;
					std::swap(indexVisites[i], indexVisites[j]);
				}
				else {
					// Si no hem aconseguit un camí més curt, tornem a l'estat anterior
					LongCamiActual = cami_aux;
				}
			}
		}

		// Si de todos los intentos es el mas corto
		if (LongCamiMesCurt < distanciaDefinitiva) {
			CamiDefinitiu = indexVisites;
			distanciaDefinitiva = LongCamiMesCurt;
		}
	}
	
	CTrack shortestTrack(&graph);

	//Fem el track a partir dels index
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
