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

void SalesmanTrackBacktrackingRec(CVertex* pActual)
{
	if (pActual == vDesti) {
		if (LenCamiActual < LenCamiMesCurt) {
			CamiCurt.Clear();
			while (!pilaCamins.empty()) {
				CamiCurt.m_Edges.push_front(pilaCamins.top());
				pilaCamins.pop();
			}
			LenCamiMesCurt = LenCamiActual;
		}
	}
	else if (LenCamiActual < LenCamiMesCurt) {
		// Pas endevant:
		pActual->m_JaHePassat = true;
		for (CEdge* pE : pActual->m_Edges) {
			if (!pE->m_pDestination->m_JaHePassat) {
				LenCamiActual += pE->m_Length;
				pilaCamins.push(pE);
				SalesmanTrackBacktrackingRec(pE->m_pDestination);
				LenCamiActual -= pE->m_Length;
			}
		}
		// Pas enrere:
		pActual->m_JaHePassat = false;
		if (!pilaCamins.empty())
			pilaCamins.pop();
		if (!pilaCamins.empty())
			pActual = pilaCamins.top()->m_pOrigin;
	}
}

CTrack SalesmanTrackBacktracking(CGraph &graph, CVisits &visits)
{
	if (graph.m_Vertices.empty())
		return CTrack(&graph);
	
	CVertex* pInici = visits.m_Vertices.front();
	vDesti = visits.m_Vertices.back();
	CamiCurt.Clear();
	LenCamiMesCurt = numeric_limits<double>::max();
	LenCamiActual = 0.0;
	for (CVertex& v : graph.m_Vertices) v.m_JaHePassat = false;

	CTrack resCami(&graph);

	for (CVertex* v : visits.m_Vertices) {
		SalesmanTrackBacktrackingRec(v);
		resCami.Append(CamiCurt);
	}
	return resCami;
}



// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	return CTrack(&graph);
}
