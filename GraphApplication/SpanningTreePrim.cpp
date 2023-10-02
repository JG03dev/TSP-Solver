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
	if (graph.m_Edges.empty() || graph.m_Vertices.empty())
		return CSpanningTree(&graph);

	struct comparator {
		bool operator()(CEdge* pE1, CEdge* pE2) {
			return pE1->m_Length > pE2->m_Length;
		}
	};
	priority_queue<CEdge*, std::vector<CEdge*>, comparator> queue;

	CSpanningTree tree(&graph);

	// Tots els vertex del graf no perteneixen a l'arbre.
	for (CVertex& v : graph.m_Vertices) v.m_PrimInTree = false;

	// Començar amb el primer vèrtex de la llista de m_Vertices.
	CVertex* firstV = &graph.m_Vertices.front();
	firstV->m_PrimInTree = true;
	
	// Agafar arestes connecatades al primer vertex.
	for (CEdge* e : firstV->m_Edges) queue.push(e);

	// Contador de vertexs del arbre.
	int count = 1;

	while (!queue.empty() && count != graph.GetNVertices()) 
	{
		// Obtenir aresta de menor cost.
		CEdge* pE = queue.top();
		queue.pop();

		// Creem un punter auxiliar on guardem el vertex que no estigui visitat.
		CVertex* auxiliarVertex = nullptr;
		if (!pE->m_pOrigin->m_PrimInTree)
			auxiliarVertex = pE->m_pOrigin;
		else
			if (!pE->m_pDestination->m_PrimInTree)
				auxiliarVertex = pE->m_pDestination;
		
		// Si tots dos estan visitats mirem una altra aresta.
		if (auxiliarVertex == nullptr)
			continue;

		// Marquem vertex com visitat, guardem aresta. 
		auxiliarVertex->m_PrimInTree = true;
		tree.m_Edges.push_back(pE);
		
		// Afegim les noves arestes a la cua.
		for (CEdge* e : auxiliarVertex->m_Edges)
			if (!e->m_pOrigin->m_PrimInTree || !e->m_pDestination->m_PrimInTree)
				queue.push(e);
		count++;
	}
	return tree;
}