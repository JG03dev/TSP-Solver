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
	double m_Length;
public:
	CBBNodeAlumne(size_t sizeVisits, int posActual)
		: m_VisitesAFer(sizeVisits, false)
		, m_sizeVisits(sizeVisits)
	{
		m_VisitesFetes.push_back(posActual);
		m_VisitesAFer[posActual] = true;
	}
	CBBNodeAlumne(const CBBNodeAlumne& node, int posActual, double distanciaNueva)
		: m_VisitesFetes(node.m_VisitesFetes)
		, m_VisitesAFer(node.m_VisitesAFer)
		, m_sizeVisits(node.m_sizeVisits)
		, m_Length(m_Length)
	{
		m_VisitesFetes.push_back(posActual);
		m_VisitesAFer[posActual] = true;
		m_Length += distanciaNueva;
	}
};

// comparator ==================================================================

struct comparator {
	bool operator()(const CBBNodeAlumne* s1, const CBBNodeAlumne* s2) {
		return s1->m_Length > s2->m_Length;
	}
};

CTrack getDijkstraPrevious(CVertex* vo, CVertex* vd)
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
				CTrack caminoDijkstra = getDijkstraPrevious(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
			}
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}

	list<int> indexCamiMesCurt;

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
		if (pV == visits.GetNVertices() - 1) { // Desti arribat!
			// Solucion encontrada
			optimum = pS->m_VisitesFetes;
			delete pS;
			break;
		}
		for () {
			double w = pS->m_Length + pE->m_Length + pDesti->m_Point.Distance(pE->m_pDestination->m_Point);
			if (w < maxLength) {
				for (CEdge* pE1 : pS->m_Track.m_Edges) {
					if (pE1->m_pOrigin == pE->m_pDestination) goto Bound;
				}
				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS);
				++SolutionNodesCreated;
				pSNew->m_Length = pS->m_Length + pE->m_Length;
				pSNew->m_WeigthMin = w;
				pSNew->m_Track.AddLast(pE);
				queue.push(pSNew);
				if (pE->m_pDestination == pDesti) maxLength = w;
			}
		Bound:;
		}
		++SolutionNodesBranched;
		delete pS;
	}
	//TODO: Pasar de vector de index a tracks del dijkstra queue
	while (!queue.empty()) {
		CBBNodeAlumne* pS = queue.top();
		queue.pop();
		//cout << pS->m_Track << endl;
		delete pS;
	}
	return optimum;



}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits &visits)
{
	return CTrack(&graph);
}

// SalesmanTrackBranchAndBound3 ===================================================


CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits &visits)
{
	return CTrack(&graph);
}
