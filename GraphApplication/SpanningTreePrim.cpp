#include "pch.h"
#include "Graph.h"
#include <vector>
#include <queue>
using namespace std;

// =============================================================================
// SpanningTreePrim ============================================================
// =============================================================================


CSpanningTree SpanningTreePrim(CGraph& graph)
{
	if (graph.m_Edges.empty() || graph.m_Vertices.empty())	return CSpanningTree(&graph);

	for (CVertex& v : graph.m_Vertices) v.m_PrimInTree = false;

	CVertex* fV = &graph.m_Vertices.front();
	fV->m_PrimInTree = true;

	//Comen�ar amb el primer v�rtex de la llista de m_Vertices
	CSpanningTree tree(&graph);

	//Cua amb prioritat d�arestes que connecten v�rtex de l�arbre amb altres que encara no s�n de l�arbre.

	struct comparator {
		bool operator()(CEdge* pE1, CEdge* pE2) {
			return pE1->m_Length > pE2->m_Length;
		}
	};
	priority_queue<CEdge*, std::vector<CEdge*>, comparator> queue;

	// Afegim arestes connectades
	for (CEdge* e : fV->m_Edges) queue.push(e);

	int vertex_afegits = 1; //Contador per comptar els vertex que hem afegit

	while (!queue.empty() && vertex_afegits < graph.GetNVertices())
	{

		CEdge* pE = queue.top();
		queue.pop();

		// Si l'origen o el dest� ja esta a l'arbre agafem la destinacio
		CVertex* pV;
		if (!pE->m_pOrigin->m_PrimInTree)
			pV = pE->m_pOrigin;
		else if (!pE->m_pDestination->m_PrimInTree)
			pV = pE->m_pDestination;
		else
			continue;
		
		//Si no esta a l'arbre afegim tant aresta com vertex i el marquem a dins del PrimInTree
		tree.Add(pE);
		pV->m_PrimInTree = true;
		vertex_afegits++;
		
		// Afegim les noves arestes 
		for (CEdge* e : pV->m_Edges)
			if (!e->m_pOrigin->m_PrimInTree || !e->m_pDestination->m_PrimInTree)
				queue.push(e);
	}

	return tree;

}
