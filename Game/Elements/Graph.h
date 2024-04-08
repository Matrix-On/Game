#pragma once
#include <vector>
#include <list>
#include <queue>


class Graph
{
public:
	Graph();

	void SetCountVertex(int vertices);
	bool AddEdge(int posSrc, int posDest);
	bool SetChip(int pos, bool chip);

	std::vector<int> GetDest(int start);
	std::vector<int> GetPath(int start, int to);

	/*std::vector<int> get_dest_bfs(int s);
	std::vector<int> get_path_bfs(int s, int t);
	std::vector<int> get_dest_bfs_ex(int s);
	std::vector<int> get_path_bfs_ex(int s, int t);*/

private:
	std::vector <std::list<int>> m_vertexConnections;
	std::vector<bool> m_chips;
	int m_countVertex;
};
