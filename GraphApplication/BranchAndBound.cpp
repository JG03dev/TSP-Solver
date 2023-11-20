#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 


// SalesmanTrackBranchAndBound1 ===================================================

// CBBNodeAlumne ===============================================================

class CBBNodeAlumne {
public:
	std::vector<int> m_VisitesFetes;
	std::vector<bool> m_VisitesAFer;
	size_t m_sizeVisits;

	//B&B1
	double m_Length;

	//B&B2
	double m_cotaInferior;
	double m_cotaSuperior;
public:
	CBBNodeAlumne(size_t sizeVisits, int posActual)
		: m_VisitesAFer(sizeVisits, false)
		, m_sizeVisits(sizeVisits)
		, m_Length(0)
	{
		m_VisitesFetes.push_back(posActual);
		m_VisitesAFer[posActual] = true;
	}
	CBBNodeAlumne(const CBBNodeAlumne& node, int posActual, double distanciaNueva)
		: m_VisitesFetes(node.m_VisitesFetes)
		, m_VisitesAFer(node.m_VisitesAFer)
		, m_sizeVisits(node.m_sizeVisits)
		, m_Length(node.m_Length)
	{
		m_VisitesFetes.push_back(posActual);
		m_VisitesAFer[posActual] = true;
		m_Length += distanciaNueva;
	}
	CBBNodeAlumne(const CBBNodeAlumne& node, int posActual, double distanciaNueva, double cotaInferior, double cotaSuperior)
		: m_VisitesFetes(node.m_VisitesFetes)
		, m_VisitesAFer(node.m_VisitesAFer)
		, m_sizeVisits(node.m_sizeVisits)
		, m_cotaInferior(cotaInferior)
		, m_cotaSuperior(cotaSuperior)
	{
		m_VisitesFetes.push_back(posActual);
		m_VisitesAFer[posActual] = true;
	}
};

// comparator ==================================================================

struct comparator {
	bool operator()(const CBBNodeAlumne* s1, const CBBNodeAlumne* s2) {
		return s1->m_Length > s2->m_Length;
	}
};

CTrack getDijkstraPrev(CVertex* vo, CVertex* vd)
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

CTrack SalesmanTrackBranchAndBound1(CGraph& graph, CVisits& visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
		return CTrack(&graph);

	// Hay que pasar por todos los que sean vertex to visit.
	for (CVertex& v : graph.m_Vertices) {
		v.m_JaHePassat = false;
	}
	
	// Definicio d'estructures
	CVertex* pInici = visits.m_Vertices.front();
	CVertex* pDesti = visits.m_Vertices.back();
	int SolutionNodesCreated = 0;
	int SolutionNodesBranched = 0;
	
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
				CTrack caminoDijkstra = getDijkstraPrev(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
			}
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}

	priority_queue<CBBNodeAlumne*, std::vector<CBBNodeAlumne*>, comparator> queue;

	// Inicialization of the first node.
	queue.push(new CBBNodeAlumne(visits.GetNVertices(), 0));
	++SolutionNodesCreated;

	double maxLength = numeric_limits<double>::max();
	std::vector<int> optimum;
	while (!queue.empty()) {
		CBBNodeAlumne* pS = queue.top();
		queue.pop();

		int pV = pS->m_VisitesFetes.back();
		if (pS->m_VisitesFetes.size() == visits.GetNVertices()) { // Desti arribat!
			// Solucion encontrada
			optimum = pS->m_VisitesFetes;
			delete pS;
			break;
		}
		else if (pS->m_VisitesFetes.size() == visits.GetNVertices() - 1) // Cuando visitados todos.
		{
			double w = pS->m_Length + matrix_dijkstra[pV][pS->m_sizeVisits - 1].second; //Distancia g (no hay heuristica)

			if (w < maxLength) {
				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, pS->m_sizeVisits - 1, matrix_dijkstra[pV][pS->m_sizeVisits - 1].second);
				++SolutionNodesCreated;
				pSNew->m_Length = pS->m_Length + matrix_dijkstra[pV][pS->m_sizeVisits - 1].second;

				maxLength = w; //Solucio parcial, actualitzem maxLength per podar
				queue.push(pSNew);
			}
		}
		else {
			for (int i = 0; i < pS->m_sizeVisits - 1; i++) {
				if (pS->m_VisitesAFer[i])  continue; //Recorrem nomes els que no hem visitat

				double w = pS->m_Length + matrix_dijkstra[pV][i].second; //Distancia g (no hay heuristica)

				if (w < maxLength) {
					CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, i, matrix_dijkstra[pV][i].second);
					++SolutionNodesCreated;
					pSNew->m_Length = pS->m_Length + matrix_dijkstra[pV][i].second;

					if (pSNew->m_VisitesFetes.size() == pSNew->m_sizeVisits) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
					queue.push(pSNew);
				}
			}
		}
		++SolutionNodesBranched;
		delete pS;
	}

	CTrack Res(&graph);
	for (int i = 1; i < optimum.size(); i++) 
	{
		Res.Append(matrix_dijkstra[optimum[i-1]][optimum[i]].first);
	}

	return Res;
}

// SalesmanTrackBranchAndBound2 ===================================================

// comparator ==================================================================

struct comparator2 {
	bool operator()(const CBBNodeAlumne* s1, const CBBNodeAlumne* s2) {
		return s1->m_cotaInferior > s2->m_cotaInferior;
	}
};

CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits &visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
		return CTrack(&graph);

	// Hay que pasar por todos los que sean vertex to visit.
	for (CVertex& v : graph.m_Vertices) {
		v.m_JaHePassat = false;
	}

	// Definicio d'estructures
	CVertex* pInici = visits.m_Vertices.front();
	CVertex* pDesti = visits.m_Vertices.back();
	int SolutionNodesCreated = 0;
	int SolutionNodesBranched = 0;

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
				CTrack caminoDijkstra = getDijkstraPrev(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
			}
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}


	// CODIGO B&B2

}

// SalesmanTrackBranchAndBound3 ===================================================


CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits &visits)
{
	return CTrack(&graph);
}
