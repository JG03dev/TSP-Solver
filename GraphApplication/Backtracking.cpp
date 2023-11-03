#include "pch.h"
#include "Graph.h"
#include <set>
#include <stack>

// =============================================================================
// SalesmanTrackBacktracking ===================================================
// =============================================================================

CVertex* vDesti;
CTrack CamiCurt(NULL);
stack<CEdge*> pilaCamins;
double LenCamiMesCurt;
double LenCamiActual;

struct NodeCami {
	CEdge* m_pEdge;
	NodeCami* m_pAnterior;
};

bool visitedAllNodes(NodeCami* pAnterior) {
	while (pAnterior)
	{
		// MIRAR SI VERTEXTOVISIT ES TRUE I JAHE PASAT ES TRUE, SI FALLA EL JAHEPASAT DELVOLVER FALSE.
	}
}

void SalesmanTrackBacktrackingRec(NodeCami* pAnterior, CVertex* pActual)
{
	if (pActual == vDesti) {
		if (visitedAllNodes(pAnterior))
		{
			if (LenCamiActual < LenCamiMesCurt) {
				CamiCurt.Clear();
				while (pAnterior) {
					CamiCurt.m_Edges.push_front(pAnterior->m_pEdge);
					pAnterior = pAnterior->m_pAnterior;
				}
				LenCamiMesCurt = LenCamiActual;
			}

		}
	}
	else if (LenCamiActual < LenCamiMesCurt) {
		pActual->m_JaHePassat = true;
		NodeCami node;
		node.m_pAnterior = pAnterior;
		for (CEdge* pE : pActual->m_Edges) {
			if (!pE->m_pDestination->m_JaHePassat) {
				node.m_pEdge = pE;
				LenCamiActual += pE->m_Length;
				SalesmanTrackBacktrackingRec(&node, pE->m_pDestination);
				LenCamiActual -= pE->m_Length;
			}
		}
		pActual->m_JaHePassat = false;
	}


	/*
	if (pActual == vDesti) {
		if (visitedAllNodes(g, visits))
		{
			if (LenCamiActual < LenCamiMesCurt) {
				CamiCurt.Clear();
				while (!pilaCamins.empty()) {
					CamiCurt.m_Edges.push_back(pilaCamins.top());
					pilaCamins.pop();
				}
				LenCamiMesCurt = LenCamiActual;
			}
		}	
	}
	else if (LenCamiActual < LenCamiMesCurt) {
		// Pas endevant:
		pActual->m_JaHePassat = true;
		NodeCami node;
		node.m_pAnterior = pAnterior;
		for (CEdge* pE : pActual->m_Edges) 
		{
			if ((pE == pilaCamins.top() && pE->m_pDestination->m_JaHePassat)) //No revisitar un node dins d'un cami d'un node a un altre
				continue;

			// ACUERDATE DE SI SE CUMPLE LA CONDICION, PONER EN g de CGRAPH en true EL JA HE PASSAT PARA QUE FUNCIONE BIEN EL VISITEDALLNODES.
			LenCamiActual += pE->m_Length;
			pilaCamins.push(pE);
			SalesmanTrackBacktrackingRec(pE->m_pDestination, &node, g, visits);
			LenCamiActual -= pE->m_Length;
		}
		// Pas enrere:
		pActual->m_JaHePassat = false;
		if (!pilaCamins.empty())
			pilaCamins.pop();
		if (!pilaCamins.empty())
			pActual = pilaCamins.top()->m_pOrigin;
	}
	*/
}

CTrack SalesmanTrackBacktracking(CGraph &graph, CVisits &visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
		return CTrack(&graph);

	// Hay que pasar por todos los que sean vertex to visit.
	for (CVertex& v : graph.m_Vertices) v.m_VertexToVisit = false;
	for (CVertex* v : visits.m_Vertices) v->m_VertexToVisit = true;
	
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


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	return CTrack(&graph);
}
