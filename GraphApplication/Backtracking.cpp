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

bool visitedAllNodes(CGraph& g, CVisits& visits) {
	for (CVertex* v : visits.m_Vertices) {
		std::string nom = v->m_Name;
		CVertex* s = g.GetVertex(nom.c_str());
		if (!s->m_JaHePassat)
			return false;
	}
}

void SalesmanTrackBacktrackingRec(CVertex* pActual, CGraph& g, CVisits &visits)
{
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
		for (CEdge* pE : pActual->m_Edges) 
		{
			if ((pE == pilaCamins.top() && pE->m_pDestination->m_JaHePassat)) //No revisitar un node dins d'un cami d'un node a un altre
				continue;

			// ACUERDATE DE SI SE CUMPLE LA CONDICION, PONER EN g de CGRAPH en true EL JA HE PASSAT PARA QUE FUNCIONE BIEN EL VISITEDALLNODES.
			LenCamiActual += pE->m_Length;
			pilaCamins.push(pE);
			SalesmanTrackBacktrackingRec(pE->m_pDestination, g, visits);
			LenCamiActual -= pE->m_Length;
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
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
		return CTrack(&graph);
	
	CVertex* pInici = visits.m_Vertices.front();
	vDesti = visits.m_Vertices.back();
	CamiCurt.Clear();
	LenCamiMesCurt = numeric_limits<double>::max();
	LenCamiActual = 0.0;

	for (CVertex& v : graph.m_Vertices) v.m_JaHePassat = false;

	SalesmanTrackBacktrackingRec(pInici, visits);

	return CamiCurt;
}



// =============================================================================
// SalesmanTrackBacktrackingGreedy =============================================
// =============================================================================


CTrack SalesmanTrackBacktrackingGreedy(CGraph& graph, CVisits& visits)
{
	return CTrack(&graph);
}
