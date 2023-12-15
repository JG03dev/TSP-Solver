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
	return CTrack(&graph);

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



	// CHANGE FROM HERE DOWN BELOW!

	std::vector<int> indexVisites(visits.GetNVertices());

	for (int i = 0; i < visits.GetNVertices(); ++i) {
		indexVisites[i] = i;
	}

	std::vector<bool> m_VisitesAFer;

	double LongCamiActual = 0.0;
	double LongCamiMesCurt = numeric_limits<double>::max();

	// Inicialitzacio aleatoria
	int origen = 0, desti = indexVisites.back();
	// Shuffle the elements in between
	std::random_device rd;
	std::mt19937 gen(rd());
	std::vector<int> middleElements(indexVisites.begin() + 1, indexVisites.end() - 1);
	std::shuffle(middleElements.begin(), middleElements.end(), gen);

	// Reconstruct the shuffled array
	indexVisites.clear();
	indexVisites.push_back(origen);
	indexVisites.insert(indexVisites.end(), middleElements.begin(), middleElements.end());
	indexVisites.push_back(desti);


	for (int i = 1; i < visits.GetNVertices() - 2; ++i) {
		for (int j = i + 1; j < visits.GetNVertices() - 1; ++j) {
			if (i == j) continue;
			int iVisites = indexVisites[i], jVisites = indexVisites[j];

			double cami_aux = LongCamiActual;

			if (iVisites != jVisites - 1 && iVisites != jVisites + 1) {
				LongCamiActual = LongCamiActual - matrix_dijkstra[iVisites - 1][iVisites].second - matrix_dijkstra[iVisites][iVisites + 1].second -
					matrix_dijkstra[jVisites - 1][jVisites].second - matrix_dijkstra[jVisites][jVisites + 1].second + matrix_dijkstra[iVisites - 1][jVisites].second + matrix_dijkstra[j][iVisites + 1].second
					+ matrix_dijkstra[jVisites - 1][iVisites].second + matrix_dijkstra[iVisites][jVisites + 1].second;
			}
			else {
				// mirar formula foto
				LongCamiActual = LongCamiActual - matrix_dijkstra[iVisites - 1][iVisites].second - matrix_dijkstra[jVisites][jVisites + 1].second +
					matrix_dijkstra[iVisites - 1][jVisites].second + matrix_dijkstra[iVisites][jVisites + 1].second;
			}

			if (LongCamiActual < LongCamiMesCurt) {
				LongCamiMesCurt = LongCamiActual;
			}
			else {
				// Si no hem aconseguit un camí més curt, tornem a l'estat anterior
				LongCamiActual = cami_aux;
			}
		}
	}

	return CTrack(&graph);
}
