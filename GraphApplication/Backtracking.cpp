#include "pch.h"
#include "Graph.h"
#include <stack>

// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

CVertex* vDesti;
CTrack CamiCurt(NULL);
double LenCamiMesCurt;
double LenCamiActual;
int toVisitCounter = 0;
int allNodes = 0;
list<int> indexVisits;

struct NodeCami {
	CEdge* m_pEdge;
	NodeCami* m_pAnterior;
};

void SalesmanTrackBacktrackingRec(NodeCami* pAnterior, CVertex* pActual)
{
	if (toVisitCounter == allNodes && pActual == vDesti && LenCamiActual < LenCamiMesCurt) {
		CamiCurt.Clear();
		while (pAnterior) {
			CamiCurt.m_Edges.push_front(pAnterior->m_pEdge);
			pAnterior = pAnterior->m_pAnterior;
		}
		LenCamiMesCurt = LenCamiActual;
		return;
	}
	else if (LenCamiActual < LenCamiMesCurt) {
		if (pActual->m_VertexToVisit && !pActual->m_JaHePassat && pActual != vDesti)
			toVisitCounter++;

		bool stateActualAux = pActual->m_JaHePassat; // Per despres tornar a l'estat anterior.
		pActual->m_JaHePassat = true;
		NodeCami node;
		node.m_pAnterior = pAnterior;

		for (CEdge* pE : pActual->m_Edges) {
			if (!pE->m_Used) {
				pE->m_Used = true;
				node.m_pEdge = pE;
				LenCamiActual += pE->m_Length;
				SalesmanTrackBacktrackingRec(&node, pE->m_pDestination);
				// Tornem a l'estat anterior!
				LenCamiActual -= pE->m_Length;
				pE->m_Used = false;
			}
		}

		// Tornem a l'estat anterior!
		pActual->m_JaHePassat = stateActualAux;
		if (pActual->m_VertexToVisit && !pActual->m_JaHePassat && pActual != vDesti)
			toVisitCounter--;
	}
}

CTrack SalesmanTrackBacktracking(CGraph& graph, CVisits& visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
		return CTrack(&graph);

	// Hay que pasar por todos los que sean vertex to visit.
	for (CVertex& v : graph.m_Vertices) {
		v.m_VertexToVisit = false;
		v.m_JaHePassat = false;
	}
	for (CVertex* v : visits.m_Vertices) v->m_VertexToVisit = true;
	for (CEdge& e : graph.m_Edges) e.m_Used = false;

	allNodes = visits.GetNVertices() - 1;
	CVertex* pInici = visits.m_Vertices.front();
	vDesti = visits.m_Vertices.back();
	CamiCurt.Clear();
	LenCamiMesCurt = numeric_limits<double>::max();
	LenCamiActual = 0.0;

	if (pInici == vDesti)
		toVisitCounter++;

	SalesmanTrackBacktrackingRec(NULL, pInici);

	return CamiCurt;
}


// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================

void SalesmanTrackBacktrackingGreedyRec(list<int>& caminoFinal, CVisits& visits, std::vector<std::vector<std::pair<CTrack, double>>>& matrix_dijkstra, CVertex* pActual)
{
	if (pActual == vDesti && indexVisits.size() != 1) // Si fulla i NO estem a l'inici (en cas de que inici i fi siguin iguals)
	{
		if (toVisitCounter == allNodes && LenCamiActual < LenCamiMesCurt)// i millor solucio trobada fins ara
		{
			LenCamiMesCurt = LenCamiActual;
			caminoFinal = indexVisits;
		}
		return;
	}
	else if (LenCamiActual < LenCamiMesCurt && !pActual->m_JaHePassat) {
		toVisitCounter++;

		bool stateActualAux = pActual->m_JaHePassat; // Per despres tornar a l'estat anterior.		
		pActual->m_JaHePassat = true;

		for (int i = 0; i < visits.m_Vertices.size(); i++) // Acces al desti
		{
			// MODIFICACION AQUI!
			if (indexVisits.back() == i)
				continue;

			// Accedim al vertex
			CVertex* pDesti = *std::next(visits.m_Vertices.begin(), i);
			if (!pDesti->m_JaHePassat || (i > 0 && pDesti == vDesti)) // Cas especial on acceptem el inici quan es tambe el desti
			{
				LenCamiActual += matrix_dijkstra[indexVisits.back()][i].second;
				indexVisits.push_back(i);
				SalesmanTrackBacktrackingGreedyRec(caminoFinal, visits, matrix_dijkstra, pDesti);

				// Tornem a l'estat anterior!
				indexVisits.pop_back();
				LenCamiActual -= matrix_dijkstra[indexVisits.back()][i].second;
			}
		}
		// Pas Endarrera
		toVisitCounter--;

		// Tornem a l'estat anterior!
		pActual->m_JaHePassat = stateActualAux;
	}
}

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

CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
		return CTrack(&graph);

	// Hay que pasar por todos los que sean vertex to visit.
	for (CVertex& v : graph.m_Vertices) {
		v.m_JaHePassat = false;
	}

	CTrack CamiMesCurt(NULL);
	LenCamiMesCurt = numeric_limits<double>::max();
	LenCamiActual = 0.0;
	CVertex* pInici = visits.m_Vertices.front();
	vDesti = visits.m_Vertices.back();
	toVisitCounter = 0;
	allNodes = visits.GetNVertices() - 1;

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
				// MODIFICACION AQUI!
				CTrack caminoDijkstra = getDijkstraPrevious(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
			}
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}

	indexVisits.push_back(0); //Index pInici
	list<int> indexCamiMesCurt;

	// Crida recursiva
	SalesmanTrackBacktrackingGreedyRec(indexCamiMesCurt, visits, matrix_dijkstra, pInici);

	//Fem el track a partir dels index
	int auxOrigen = 0;
	for (int i : indexCamiMesCurt)
	{
		if (i == 0)
			continue;
		CamiMesCurt.Append(matrix_dijkstra[auxOrigen][i].first);
		auxOrigen = i;
	}

	return CamiMesCurt;
}