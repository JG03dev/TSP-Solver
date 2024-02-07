#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 
#include <random>
#include <algorithm>
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

	// Vector inicial que inicialitzara els vectors d'indexos
	std::vector<int> indexInicial(visits.GetNVertices(), 0);

	for (int i = 0; i < visits.GetNVertices(); ++i) {
		indexInicial[i] = i;
	}

	// Randomitzador
	std::random_device rd;

	// Total d'intents (proporcional al numero de visites)
	int total_tries = visits.GetNVertices() * visits.GetNVertices() * 10'000;

	// Cami i distancia final de tots els intents, inicialitzats en ordre de les visites
	vector<int> CamiDefinitiu = indexInicial;

	double distanciaDefinitiva = 0.0;
	for (int i = 1; i < CamiDefinitiu.size(); i++) {
		distanciaDefinitiva += matrix_dijkstra[CamiDefinitiu[i-1]][CamiDefinitiu[i]].second;
	}

	for (int tries = 0; tries < total_tries; tries++) {

		std::vector<int> indexVisites = indexInicial;

		// ------------------------- Inicialitzacio aleatoria -------------------------
		int origen = 0, desti = indexVisites.back();
		// Shuffle the elements in between
		std::srand(rd());
		std::random_shuffle(indexVisites.begin() + 1, indexVisites.end() - 1);
		// --------------------------------------------------------------------------- 

		double LongCamiActual = 0.0;

		for (int i = 1; i < indexVisites.size(); i++) {
			LongCamiActual += matrix_dijkstra[indexVisites[i-1]][indexVisites[i]].second;
		}

		double LongCamiMesCurt = LongCamiActual;

		for (int i = 1; i < visits.GetNVertices() - 2; ++i) {
			for (int j = i + 1; j < visits.GetNVertices() - 1; ++j) {

				double cami_aux = LongCamiActual;

				if (j != i + 1)
				{
					LongCamiActual = LongCamiActual
						- matrix_dijkstra[indexVisites[i - 1]][indexVisites[i]].second - matrix_dijkstra[indexVisites[i]][indexVisites[i + 1]].second
						- matrix_dijkstra[indexVisites[j - 1]][indexVisites[j]].second - matrix_dijkstra[indexVisites[j]][indexVisites[j + 1]].second
						+ matrix_dijkstra[indexVisites[i - 1]][indexVisites[j]].second + matrix_dijkstra[indexVisites[j]][indexVisites[i + 1]].second
						+ matrix_dijkstra[indexVisites[j - 1]][indexVisites[i]].second + matrix_dijkstra[indexVisites[i]][indexVisites[j + 1]].second;
				}
				else
				{
					LongCamiActual = LongCamiActual
						- matrix_dijkstra[indexVisites[i - 1]][indexVisites[i]].second - matrix_dijkstra[indexVisites[j]][indexVisites[j + 1]].second
						+ matrix_dijkstra[indexVisites[i - 1]][indexVisites[j]].second + matrix_dijkstra[indexVisites[i]][indexVisites[j + 1]].second;
				}
				
				if (LongCamiActual < LongCamiMesCurt) {
					LongCamiMesCurt = LongCamiActual;
					std::swap(indexVisites[i], indexVisites[j]);
				}
				else {
					// Si no hem aconseguit un cam� m�s curt, tornem a l'estat anterior
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
