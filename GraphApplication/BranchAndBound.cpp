#include "pch.h"
#include "Graph.h"
#include <queue>
#include <iostream>
#include <iomanip> 

#define PRECISION 1e-5

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
	//Inicializador inicial
	CBBNodeAlumne(size_t sizeVisits, int posActual, int cotaInferior=0, int cotaSuperior=0)
		: m_VisitesAFer(sizeVisits, false)
		, m_sizeVisits(sizeVisits)
		, m_Length(0)
		, m_cotaInferior(cotaInferior)
		, m_cotaSuperior(cotaSuperior)
	{
		m_VisitesFetes.emplace_back(posActual);
		m_VisitesAFer[posActual] = true;
	}
	//Inicializador general B&B1
	CBBNodeAlumne(const CBBNodeAlumne& node, int posActual, double distanciaNueva)
		: m_VisitesFetes(node.m_VisitesFetes)
		, m_VisitesAFer(node.m_VisitesAFer)
		, m_sizeVisits(node.m_sizeVisits)
		, m_Length(node.m_Length)
	{
		m_VisitesFetes.emplace_back(posActual);
		m_VisitesAFer[posActual] = true;
		m_Length += distanciaNueva;
	}
	//Inicializador general B&B2
	CBBNodeAlumne(const CBBNodeAlumne& node, int posActual, double distanciaNueva, double cotaInferior, double cotaSuperior)
		: m_VisitesFetes(node.m_VisitesFetes)
		, m_VisitesAFer(node.m_VisitesAFer)
		, m_sizeVisits(node.m_sizeVisits)
		, m_cotaInferior(cotaInferior)
		, m_cotaSuperior(cotaSuperior)
	{
		m_VisitesFetes.emplace_back(posActual);
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

	double maxLength = numeric_limits<double>::max();
	std::vector<int> optimum;
	while (!queue.empty()) {
		CBBNodeAlumne* pS = queue.top();
		queue.pop();

		int pV = pS->m_VisitesFetes.back(); // Ultim node del cami.
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
					pSNew->m_Length = pS->m_Length + matrix_dijkstra[pV][i].second;

					if (pSNew->m_VisitesFetes.size() == pSNew->m_sizeVisits) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
					queue.push(pSNew);
				}
			}
		}
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

struct comparator2 {
	bool operator()(const CBBNodeAlumne* s1, const CBBNodeAlumne* s2) {
		return s1->m_cotaInferior + s1->m_Length > s2->m_cotaInferior + s2->m_Length;
	}
};

CTrack SalesmanTrackBranchAndBound2(CGraph& graph, CVisits &visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2 || graph.m_Vertices.size() >= 100)
		return CTrack(&graph);

	std::vector<std::vector<std::pair<CTrack, double>>> matrix_dijkstra;
	std::pair<double, double>init(std::numeric_limits<double>::max(), 0.0); //first = min, second = max
	std::vector<std::pair<double, double>> vector_minmax(visits.m_Vertices.size(), init);
	for (CVertex* vo : visits.m_Vertices) // Vertex Origen
	{
		DijkstraQueue(graph, vo);
		std::vector<std::pair<CTrack, double>> vector_dijkstra;
		int j = 0;
		for (CVertex* vd : visits.m_Vertices) // Vertex Desti
		{
			if (vo == vd) {
				vector_dijkstra.emplace_back(std::make_pair(CTrack(NULL), 0.0));
			}
			else {
				CTrack caminoDijkstra = getDijkstraPrev(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
				if (vd->m_DijkstraDistance < vector_minmax[j].first)
					vector_minmax[j].first = vd->m_DijkstraDistance;
				if (vd->m_DijkstraDistance > vector_minmax[j].second)
					vector_minmax[j].second = vd->m_DijkstraDistance;
			}
			j++;
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}

	// CODIGO B&B2
	priority_queue<CBBNodeAlumne*, std::vector<CBBNodeAlumne*>, comparator2> queue;

	// Inicialization of the first node.
	CBBNodeAlumne* first = new CBBNodeAlumne(visits.GetNVertices(), 0);
	//Calcul cota inferior/superior
	//Per cada node restant
	for (int i = 1; i < first->m_sizeVisits; i++)
	{
		first->m_cotaInferior += vector_minmax[i].first;
		first->m_cotaSuperior += vector_minmax[i].second;
	}
	queue.push(first);

	double maxLength = numeric_limits<double>::max()
		, cotaSupMin = numeric_limits<double>::max();

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
			CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, pS->m_sizeVisits - 1, matrix_dijkstra[pV][pS->m_sizeVisits - 1].second,
				pS->m_cotaInferior, pS->m_cotaSuperior);
			//calcular cotes
			pSNew->m_cotaInferior += matrix_dijkstra[pSNew->m_VisitesFetes[pSNew->m_VisitesFetes.size() - 2]][pSNew->m_VisitesFetes.back()].second - vector_minmax[pSNew->m_VisitesFetes.back()].first;
			pSNew->m_cotaSuperior += matrix_dijkstra[pSNew->m_VisitesFetes[pSNew->m_VisitesFetes.size() - 2]][pSNew->m_VisitesFetes.back()].second - vector_minmax[pSNew->m_VisitesFetes.back()].second + PRECISION;


			if (w < maxLength && pSNew->m_cotaInferior <= cotaSupMin) {
				
				pSNew->m_Length = pS->m_Length + matrix_dijkstra[pV][pS->m_sizeVisits - 1].second;

				maxLength = w; //Solucio parcial, actualitzem maxLength per podar
				if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
				queue.push(pSNew);
			}
			else
				delete pSNew;
		}
		else {
			for (int i = 0; i < pS->m_sizeVisits - 1; i++) {
				if (pS->m_VisitesAFer[i])  continue; //Recorrem nomes els que no hem visitat

				double w = pS->m_Length + matrix_dijkstra[pV][i].second; //Distancia g (no hay heuristica)

				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, i, matrix_dijkstra[pV][i].second,
					pS->m_cotaInferior, pS->m_cotaSuperior);
				//Calcular Cotes
				pSNew->m_cotaInferior += matrix_dijkstra[pSNew->m_VisitesFetes[pSNew->m_VisitesFetes.size() - 2]][pSNew->m_VisitesFetes.back()].second - vector_minmax[pSNew->m_VisitesFetes.back()].first;
				pSNew->m_cotaSuperior += matrix_dijkstra[pSNew->m_VisitesFetes[pSNew->m_VisitesFetes.size() - 2]][pSNew->m_VisitesFetes.back()].second - vector_minmax[pSNew->m_VisitesFetes.back()].second + PRECISION;

				if (w < maxLength && pSNew->m_cotaInferior <= cotaSupMin) {

					pSNew->m_Length = pS->m_Length + matrix_dijkstra[pV][i].second;

					if (pSNew->m_VisitesFetes.size() == pSNew->m_sizeVisits) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
					if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
					queue.push(pSNew);
				}
				else
					delete pSNew;
			}
		}
		delete pS;
	}

	CTrack Res(&graph);
	for (int i = 1; i < optimum.size(); i++)
	{
		Res.Append(matrix_dijkstra[optimum[i - 1]][optimum[i]].first);
	}
	return Res;
}

// SalesmanTrackBranchAndBound3 ===================================================

std::vector<std::pair<double, double>> calcularCotes(std::vector<std::vector<std::pair<CTrack, double>>> matrix_dijkstra, std::vector<std::pair<double, double>> vmm, CBBNodeAlumne* n)
{
	//Cas cami buit
	if (n->m_VisitesFetes.size() <= 1)
	{
		//Per cada node restant
		for (int i = 1; i < n->m_sizeVisits; i++)
		{
			n->m_cotaInferior += vmm[i].first;
			n->m_cotaSuperior += vmm[i].second;
		}
	}
	else //Cas afegim node
	{
		for (int i = 0; i < n->m_sizeVisits; i++) // Vertex Origen
		{
			if (n->m_VisitesAFer[i] && i != n->m_VisitesFetes.back()) continue; // Si esta visitat.
			
			for (int j = 0; j < n->m_sizeVisits; j++) // Vertex Desti
			{
				if (i == j || !n->m_VisitesAFer[j] || 
					(i == n->m_VisitesFetes.back() && j == n->m_sizeVisits - 1 && n->m_VisitesFetes.size() < n->m_sizeVisits - 1)) 
					continue;

				double dis = matrix_dijkstra[i][j].second;

				if (dis < vmm[j].first)
					vmm[j].first = dis;
				if (dis > vmm[j].second)
					vmm[j].second = dis;
			}
		}
		n->m_cotaInferior += matrix_dijkstra[n->m_VisitesFetes[n->m_VisitesFetes.size() - 2]][n->m_VisitesFetes.back()].second - vmm[n->m_VisitesFetes.back()].first;
		n->m_cotaSuperior += matrix_dijkstra[n->m_VisitesFetes[n->m_VisitesFetes.size() - 2]][n->m_VisitesFetes.back()].second - vmm[n->m_VisitesFetes.back()].second + PRECISION;
	}
	return vmm;
}


struct comparator3 {
	bool operator()(const CBBNodeAlumne* s1, const CBBNodeAlumne* s2) {
		return s1->m_cotaInferior + s1->m_Length > s2->m_cotaInferior + s2->m_Length;
	}
};

CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits &visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2 || graph.m_Vertices.size() >= 100)
		return CTrack(&graph);

	std::vector<std::vector<std::pair<CTrack, double>>> matrix_dijkstra;
	std::pair<double, double>init(std::numeric_limits<double>::max(), 0.0); //first = min, second = max
	std::vector<std::pair<double, double>> vector_minmax(visits.m_Vertices.size(), init);
	for (CVertex* vo : visits.m_Vertices) // Vertex Origen
	{
		DijkstraQueue(graph, vo);
		std::vector<std::pair<CTrack, double>> vector_dijkstra;
		int j = 0;
		for (CVertex* vd : visits.m_Vertices) // Vertex Desti
		{
			if (vo == vd) {
				vector_dijkstra.emplace_back(std::make_pair(CTrack(NULL), 0.0));
			}
			else {
				CTrack caminoDijkstra = getDijkstraPrev(vo, vd);
				vector_dijkstra.emplace_back(std::pair<CTrack, double>(caminoDijkstra, vd->m_DijkstraDistance));
				if (vd->m_DijkstraDistance < vector_minmax[j].first)
					vector_minmax[j].first = vd->m_DijkstraDistance;
				if (vd->m_DijkstraDistance > vector_minmax[j].second)
					vector_minmax[j].second = vd->m_DijkstraDistance;
			}
			j++;
		}
		matrix_dijkstra.push_back(vector_dijkstra);
	}

	// CODIGO B&B3
	priority_queue<CBBNodeAlumne*, std::vector<CBBNodeAlumne*>, comparator2> queue;

	// Inicialization of the first node.
	CBBNodeAlumne* first = new CBBNodeAlumne(visits.GetNVertices(), 0);
	
	// Calcul cota inferior/superior
	// Per cada node restant
	for (int i = 1; i < first->m_sizeVisits; i++)
	{
		first->m_cotaInferior += vector_minmax[i].first;
		first->m_cotaSuperior += vector_minmax[i].second;
	}
	queue.push(first);

	double maxLength = numeric_limits<double>::max()
		, cotaSupMin = numeric_limits<double>::max();

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
			CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, pS->m_sizeVisits - 1, matrix_dijkstra[pV][pS->m_sizeVisits - 1].second,
				pS->m_cotaInferior, pS->m_cotaSuperior);
			//calcular cotes
			vector_minmax = calcularCotes(matrix_dijkstra, std::vector<std::pair<double, double>>(visits.m_Vertices.size(), init), pSNew);

			
			if (w < maxLength && pSNew->m_cotaInferior <= cotaSupMin) {

				pSNew->m_Length = pS->m_Length + matrix_dijkstra[pV][pS->m_sizeVisits - 1].second;

				maxLength = w; //Solucio parcial, actualitzem maxLength per podar
				if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
				queue.push(pSNew);
			}
			else
				delete pSNew;
		}
		else {
			for (int i = 0; i < pS->m_sizeVisits - 1; i++) {
				if (pS->m_VisitesAFer[i])  continue; //Recorrem nomes els que no hem visitat

				double w = pS->m_Length + matrix_dijkstra[pV][i].second; //Distancia g (no hay heuristica)

				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, i, matrix_dijkstra[pV][i].second,
					pS->m_cotaInferior, pS->m_cotaSuperior);
				//Calcular Cotes				
				vector_minmax = calcularCotes(matrix_dijkstra, std::vector<std::pair<double, double>>(visits.m_Vertices.size(), init), pSNew);

				if (w < maxLength && pSNew->m_cotaInferior <= cotaSupMin) {

					pSNew->m_Length = pS->m_Length + matrix_dijkstra[pV][i].second;

					if (pSNew->m_VisitesFetes.size() == pSNew->m_sizeVisits) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
					if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
					queue.push(pSNew);
				}
				else
					delete pSNew;
			}
		}
		delete pS;
	}

	CTrack Res(&graph);
	for (int i = 1; i < optimum.size(); i++)
	{
		Res.Append(matrix_dijkstra[optimum[i - 1]][optimum[i]].first);
	}
	return Res;
}
