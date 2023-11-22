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
	int m_numFets;

	//B&B1
	double m_Length;

	//B&B2
	double m_cotaInferior;
	double m_cotaSuperior;
public:
	//Inicializador inicial
	CBBNodeAlumne(size_t sizeVisits, int posActual, int cotaInferior=0, int cotaSuperior=0)
		: m_VisitesAFer(sizeVisits, false)
		, m_VisitesFetes(sizeVisits, -1)
		, m_numFets(0)
		, m_sizeVisits(sizeVisits)
		, m_Length(0)
		, m_cotaInferior(cotaInferior)
		, m_cotaSuperior(cotaSuperior)
	{
		m_VisitesFetes[m_numFets] = posActual;
		m_VisitesAFer[posActual] = true;
		m_numFets++;
	}
	//Inicializador general B&B1
	CBBNodeAlumne(const CBBNodeAlumne& node, int posActual, double distanciaNueva)
		: m_VisitesFetes(node.m_VisitesFetes)
		, m_VisitesAFer(node.m_VisitesAFer)
		, m_numFets(node.m_numFets)
		, m_sizeVisits(node.m_sizeVisits)
		, m_Length(distanciaNueva)
	{
		m_VisitesFetes[m_numFets] = posActual;
		m_VisitesAFer[posActual] = true;
		m_numFets++;
	}
	//Inicializador general B&B2
	CBBNodeAlumne(const CBBNodeAlumne& node, int posActual, double distanciaNueva, double cotaInferior, double cotaSuperior)
		: m_VisitesFetes(node.m_VisitesFetes)
		, m_VisitesAFer(node.m_VisitesAFer)
		, m_numFets(node.m_numFets)
		, m_sizeVisits(node.m_sizeVisits)
		, m_cotaInferior(cotaInferior)
		, m_cotaSuperior(cotaSuperior)
		, m_Length(distanciaNueva)
	{
		m_VisitesFetes[m_numFets] = posActual;
		m_VisitesAFer[posActual] = true;
		m_numFets++;
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

		int pV = pS->m_VisitesFetes[pS->m_numFets - 1]; // Ultim node del cami.
		if (pS->m_VisitesFetes.back() != -1) { // Desti arribat!
			// Solucion encontrada
			optimum = pS->m_VisitesFetes;
			delete pS;
			break;
		}
		else if (pS->m_VisitesFetes[visits.GetNVertices() - 2] != - 1) // Cuando visitados todos.
		{
			double realDist = matrix_dijkstra[pV][pS->m_sizeVisits - 1].second;
			double w = pS->m_Length + realDist; //Distancia g (no hay heuristica)

			if (w < maxLength) {
				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, pS->m_sizeVisits - 1, w);

				maxLength = w; //Solucio parcial, actualitzem maxLength per podar
				queue.push(pSNew);
			}
		}
		else {
			for (int i = 0; i < pS->m_sizeVisits - 1; i++) {
				if (pS->m_VisitesAFer[i])  continue; //Recorrem nomes els que no hem visitat
				
				double realDist = matrix_dijkstra[pV][i].second;
				double w = pS->m_Length + realDist; //Distancia g (no hay heuristica)

				if (w < maxLength) {
					CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, i, w);
					if (pSNew->m_VisitesFetes.back() != -1) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
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
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
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

		int pV = pS->m_VisitesFetes[pS->m_numFets - 1];
		if (pS->m_VisitesFetes.back() != -1) { // Desti arribat!
			// Solucion encontrada
			optimum = pS->m_VisitesFetes;
			delete pS;
			break;
		}
		else if (pS->m_VisitesFetes[visits.GetNVertices() - 2] != -1) // Cuando visitados todos.
		{
			double distReal = matrix_dijkstra[pV][pS->m_sizeVisits - 1].second;
			double w = pS->m_Length + distReal; //Distancia g (no hay heuristica)
			
			//calcular cotes
			double cotaInferior = pS->m_cotaInferior + distReal - vector_minmax[pS->m_sizeVisits - 1].first;
			double cotaSuperior = pS->m_cotaSuperior + distReal - vector_minmax[pS->m_sizeVisits - 1].second + PRECISION;

			if (w < maxLength && cotaInferior <= cotaSupMin) {
				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, pS->m_sizeVisits - 1, w,
					cotaInferior, cotaSuperior);


				maxLength = w; //Solucio parcial, actualitzem maxLength per podar
				if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
				queue.push(pSNew);
			}
		}
		else {
			for (int i = 0; i < pS->m_sizeVisits - 1; i++) {
				if (pS->m_VisitesAFer[i])  continue; //Recorrem nomes els que no hem visitat

				double distReal = matrix_dijkstra[pV][i].second;
				double w = pS->m_Length + distReal; //Distancia g (no hay heuristica)

				//calcular cotes
				double cotaInferior = pS->m_cotaInferior + distReal - vector_minmax[i].first;
				double cotaSuperior = pS->m_cotaSuperior + distReal - vector_minmax[i].second + PRECISION;

				if (w < maxLength && cotaInferior <= cotaSupMin) {

					CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, i, w,
						cotaInferior, cotaSuperior);

					if (pSNew->m_VisitesFetes.back() != -1) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
					if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
					queue.push(pSNew);
				}
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

struct comparator3 {
	bool operator()(const CBBNodeAlumne* s1, const CBBNodeAlumne* s2) {
		return s1->m_cotaInferior + s1->m_Length > s2->m_cotaInferior + s2->m_Length;
	}
};

CTrack SalesmanTrackBranchAndBound3(CGraph& graph, CVisits &visits)
{
	if (graph.m_Vertices.size() < 2 || visits.m_Vertices.size() < 2)
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

		int pV = pS->m_VisitesFetes[pS->m_numFets - 1];
		if (pS->m_VisitesFetes.back() != -1) { // Desti arribat!
			// Solucion encontrada
			optimum = pS->m_VisitesFetes;
			delete pS;
			break;
		}
		else if (pS->m_VisitesFetes[visits.GetNVertices() - 2] != -1) // Cuando visitados todos.
		{
			double distReal = matrix_dijkstra[pV][pS->m_sizeVisits - 1].second;
			double w = pS->m_Length + distReal; //Distancia g (no hay heuristica)

			//Nota: m'he donat compte que en aquest cas l'heuristica es que no hi han heuristiques XD
			double cotaInferior = w
				, cotaSuperior = cotaInferior + PRECISION; //Teoricament son iguals, a la practica sumem la precisio al superior

			if (w < maxLength && cotaInferior <= cotaSupMin) {
				CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, pS->m_sizeVisits - 1, w,
					cotaInferior, cotaSuperior);

				maxLength = w; //Solucio parcial, actualitzem maxLength per podar
				if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
				queue.push(pSNew);
			}
		}
		else {
			for (int i = 0; i < pS->m_sizeVisits - 1; i++) {
				if (pS->m_VisitesAFer[i])  continue; //Recorrem nomes els que no hem visitat

				double distReal = matrix_dijkstra[pV][i].second;
				double w = pS->m_Length + distReal; //Distancia g (no hay heuristica)

				vector_minmax = std::vector<std::pair<double, double>>(visits.m_Vertices.size(), init);
				// Recalcular minims maxims
				for (int i = 0; i < pS->m_sizeVisits; i++) // Vertex Origen
				{
					if (pS->m_VisitesAFer[i]) continue; // No tenim en compte els que d'origen ja s'han visitat.

					for (int j = 0; j < pS->m_sizeVisits; j++) // Vertex Desti
					{
						if (i == j || pS->m_VisitesAFer[j] ||
						(i == pV && j == pS->m_sizeVisits - 1)) //Si no estem al penultim node no te sentit mirar d'anar a l'ultim
							continue;

						double dis = matrix_dijkstra[i][j].second;

						if (dis < vector_minmax[j].first)
							vector_minmax[j].first = dis;
						if (dis > vector_minmax[j].second)
							vector_minmax[j].second = dis;
					}
				}

				//calcular cotes
				double cotaInferior = 0.0, cotaSuperior = 0.0;

				//Sumar distancia heuristica de nomes els nodes NO visitats
				for (int i = 1; i < pS->m_sizeVisits; i++)
				{
					if (pS->m_VisitesAFer[i] || i == pV) continue; //Saltem els NO visitats (incloent l'actual)
					cotaInferior += vector_minmax[i].first;
					cotaSuperior += vector_minmax[i].second;
				}
				//Sumar distancia real acumulada dels visitats (i precisio)
				cotaInferior += w; cotaSuperior += w + PRECISION;

				if (w < maxLength && cotaInferior <= cotaSupMin) {

					CBBNodeAlumne* pSNew = new CBBNodeAlumne(*pS, i, w,
						cotaInferior, cotaSuperior);

					if (pSNew->m_VisitesFetes.back() != -1) maxLength = w; //Solucio parcial, actualitzem maxLength per podar
					if (pSNew->m_cotaSuperior < cotaSupMin) cotaSupMin = pSNew->m_cotaSuperior;
					queue.push(pSNew);
				}
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
