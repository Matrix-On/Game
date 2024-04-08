#include "Graph.h"

Graph::Graph()
	:
	m_countVertex(-1)
{}

void Graph::SetCountVertex(int vertices)
{
	m_countVertex = vertices;

	m_vertexConnections.clear();
	m_chips.clear();

	m_vertexConnections.resize(m_countVertex);
	m_chips.resize(m_countVertex, false);
}

bool Graph::AddEdge(int posSrc, int posDest)
{
	if (m_countVertex == -1) return false;

	m_vertexConnections[posSrc].push_back(posDest);
	m_vertexConnections[posDest].push_back(posSrc);

	return true;
}

bool Graph::SetChip(int pos, bool chip)
{
	if (m_countVertex == -1) return false;

	m_chips[pos] = chip;

	return true;
}

//std::vector<int> Graph::get_path_bfs(int s, int t)
//{
//	std::vector<int> dist(_n_vertex, _n_vertex);
//	std::vector<int> p(_n_vertex, -1);
//	std::queue<int> q;
//	dist[s] = 0;
//	q.push(s);
//
//	while (!q.empty())
//	{
//		int v = q.front();
//		q.pop();
//		for (int u : _adj_vertex[v])
//		{
//			if (dist[u] > dist[v] + 1)
//			{
//				p[u] = v;
//				dist[u] = dist[v] + 1;
//				q.push(u);
//			}
//		}
//	}
//
//	if (dist[t] == _n_vertex)
//		return {};
//
//	std::vector<int> path;
//	while (t != -1)
//	{
//		path.push_back(t);
//		t = p[t];
//	}
//
//	reverse(path.begin(), path.end());
//	return path;
//}
//
//std::vector<int> Graph::get_dest_bfs(int s)
//{
//	std::vector<int> dist(_n_vertex, _n_vertex);
//	std::queue<int> q;
//	dist[s] = 0;
//	q.push(s);
//
//	while (!q.empty())
//	{
//		int v = q.front();
//		q.pop();
//		for (int u : _adj_vertex[v])
//		{
//			if (dist[u] > dist[v] + 1)
//			{
//				dist[u] = dist[v] + 1;
//				q.push(u);
//			}
//		}
//	}
//
//	return dist;
//}
//

std::vector<int> Graph::GetPath(int start, int to)
{
	std::vector<int> dist(m_countVertex, m_countVertex);
	std::vector<int> fullPaths(m_countVertex, -1);
	std::queue<int> q;

	dist[start] = 0;
	q.push(start);

	while (!q.empty())
	{
		int elem = q.front();
		q.pop();
		for (int elemList : m_vertexConnections[elem])
		{
			if (m_chips[elemList])
				continue;

			if (dist[elemList] > dist[elem] + 1)
			{
				fullPaths[elemList] = elem;
				dist[elemList] = dist[elem] + 1;
				q.push(elemList);
			}
		}
	}

	if (dist[to] == m_countVertex)
		return {};

	std::vector<int> path;
	while (to != -1)
	{
		path.push_back(to);
		to = fullPaths[to];
	}
	path.pop_back();

	reverse(path.begin(), path.end());
	return path;
}

std::vector<int> Graph::GetDest(int s)
{
	std::vector<int> dist(m_countVertex, m_countVertex);
	std::queue<int> q;
	dist[s] = 0;
	q.push(s);
	std::vector<int> ret;

	while (!q.empty())
	{
		int elem = q.front();
		q.pop();
		for (int elemList : m_vertexConnections[elem])
		{
			if (m_chips[elemList])
				continue;

			if (dist[elemList] > dist[elem] + 1)
			{
				dist[elemList] = dist[elem] + 1;
				ret.push_back(elemList);
				q.push(elemList);
			}
		}
	}

	return ret;
}