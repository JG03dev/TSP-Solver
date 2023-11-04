#include "pch.h"
#include "Graph.h"

// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

CVertex* vDesti;
CTrack CamiCurt(NULL);
double LenCamiMesCurt;
double LenCamiActual;
int toVisitCounter = 0;
int allNodes = 0;

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

	allNodes = visits.GetNVertices()-1;
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

// CTrack SalesmanTrackBacktrackingGreedyRec(CTrack& caminoFinal, CVisits& visits, std::vector<std::vector<std::pair<CTrack, double>>> matrix_dijkstra)
// {
// 	// Accedim al vertex
// 	auto l_front = visits.m_Vertices.begin();
// 	std::advance(l_front, indexVisits.top());
// 	CVertex* pActual = *l_front; 

// 	if (toVisitCounter == allNodes && pActual == vDesti && LenCamiActual < LenCamiMesCurt) // Si fulla i solucio completa
// 	{
// 		LenCamiMesCurt = LenCamiActual;
// 		caminoFinal.Clear();
// 	}
// 	else if (LenCamiActual < LenCamiMesCurt && !pActual->m_JaHePassat) {
// 		toVisitCounter++;

// 		bool stateActualAux = pActual->m_JaHePassat; // Per despres tornar a l'estat anterior.
// 		pActual->m_JaHePassat = true;
// 		NodeCami node;
// 		node.m_pAnterior = pAnterior;
// 		for (CEdge* pE : pActual->m_Edges) {
// 			if (!pE->m_Used) {
// 				pE->m_Used = true;
// 				node.m_pEdge = pE;
// 				LenCamiActual += pE->m_Length;
// 				SalesmanTrackBacktrackingGreedyRec(&node, pE->m_pDestination);
// 				// Tornem a l'estat anterior!
// 				LenCamiActual -= pE->m_Length;
// 				pE->m_Used = false;
// 			}
// 		}

// 		// Tornem a l'estat anterior!
// 		pActual->m_JaHePassat = stateActualAux;
// 		if (pActual->m_VertexToVisit && !pActual->m_JaHePassat && pActual != vDesti)
// 			toVisitCounter--;
// 	}
// }

// CTrack getDijkstraPrevious(CGraph* pGraph, CVertex* vo, CVertex* vd)
// {
// 	CTrack camino = CTrack(pGraph);
// 	CVertex* auxVertex = vd;
// 	camino.AddFirst(auxVertex->m_pDijkstraPrevious);
// 	while (auxVertex != vo)
// 	{
// 		// Recorremos los DijkstraPrevious
// 		if (auxVertex == auxVertex->m_pDijkstraPrevious->m_pDestination)
// 			auxVertex = auxVertex->m_pDijkstraPrevious->m_pOrigin;
// 		else
// 			auxVertex = auxVertex->m_pDijkstraPrevious->m_pDestination;

// 		// Añadimos vertex de tal manera que al final quede vo->...->vd
// 		camino.AddFirst(auxVertex->m_pDijkstraPrevious);
// 	}
// 	return camino;
// }

CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
// 	// Calcular matriu de Dijkstras entre visits.
// 	CTrack CamiMesCurt(NULL);
// 	LenCamiMesCurt = numeric_limits<double>::max();
// 	LenCamiActual = 0.0;
// 	CVertex* pInici = visits.m_Vertices.front();
// 	vDesti = visits.m_Vertices.back();

// 	std::vector<std::vector<std::pair<CTrack, double>>> matrix_dijkstra;
// 	for (CVertex* vo : visits.m_Vertices) // Vertex Origen
// 	{
// 		if (vo == vDesti)
// 			continue; // basicament no cal fer-ho pel desti, fem continue per si desti es igual a origen

// 		Dijkstra(graph, vo);
// 		std::vector<std::pair<CTrack, double>> vector_dijkstra;
// 		for (CVertex* vd : visits.m_Vertices) // Vertex Desti
// 		{
// 			//Recuperar el camino de dijkstra que va de vo a vd
// 			CTrack caminoDijkstra = getDijkstraPrevious(&graph, vo, vd);

// 			vector_dijkstra.push_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
// 		}
// 		matrix_dijkstra.push_back(vector_dijkstra);
// 	}

// 	for (CVertex* v : visits.m_Vertices) v->m_JaHePassat = false;

// 	indexVisits.push(0); //Index pInici

// 	// Crida recursiva
// 	SalesmanTrackBacktrackingGreedyRec(CamiMesCurt, visits, pInici, matrix_dijkstra);

 	return CTrack(&graph);
}
