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
	if (toVisitCounter == allNodes && pActual == vDesti) {
		if (LenCamiActual < LenCamiMesCurt) {
			CamiCurt.Clear();
			while (pAnterior) {
				CamiCurt.m_Edges.push_front(pAnterior->m_pEdge);
				pAnterior = pAnterior->m_pAnterior;
			}
			LenCamiMesCurt = LenCamiActual;
		}
	}
	else if (LenCamiActual < LenCamiMesCurt) {
		if (pActual->m_VertexToVisit && !pActual->m_JaHePassat)
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
		if (pActual->m_VertexToVisit && !pActual->m_JaHePassat)
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

	SalesmanTrackBacktrackingRec(NULL, pInici);

	return CamiCurt;
}



// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================

void SalesmanTrackBacktrackingGreedyRec()
{

}


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	// Calcular matriu de Dijkstras entre visits.
	dijk

	// Crida recursiva

	return CTrack(&graph);
}
