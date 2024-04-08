#include "Config.h"
#include <fstream>
#include <sstream>

Config::Config() {}

Config::~Config() {}

positionVertex& Config::splitVertex(const std::string& s, char delimer, positionVertex& vertex)
{
	std::stringstream ss(s);
	std::string item;

	if (std::getline(ss, item, delimer))
	{
		vertex.x = std::stof(item);
	}

	if (std::getline(ss, item, delimer))
	{
		vertex.y = std::stof(item);
	}

	return vertex;
}

positionEdge& Config::splitEdge(const std::string& s, char delimer, positionEdge& edge)
{
	std::stringstream ss(s);
	std::string item;

	if (std::getline(ss, item, delimer))
	{
		edge.f = std::stoi(item) - 1;
	}

	if (std::getline(ss, item, delimer))
	{
		edge.s = std::stoi(item) - 1;
	}

	return edge;
}

std::vector<int>& Config::split(const std::string& s, char delimer, std::vector<int>& elems)
{
	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delimer))
	{
		elems.push_back(std::stoi(item) - 1);
	}

	return elems;
}

std::optional<configureGame> Config::Parse(const char* path)
{
	configureGame res{};

	std::fstream file;
	file.open(path, std::ios::in);

	if (!file.is_open())
	{
		std::printf("Error!\nDon`t open level: %s\n", path);
		return {};
	}

	try
	{
		std::string chipCount;
		file >> chipCount;
		res.m_chipCount = std::stoi(chipCount);
		res.m_positionChips.reserve(res.m_chipCount);
		res.m_winPositionChips.reserve(res.m_chipCount);

		std::string vertexCount;
		file >> vertexCount;
		res.m_vertexCount = std::stoi(vertexCount);
		res.m_vecPositionVertex.reserve(res.m_vertexCount);

		for (int i = 0; i < res.m_vertexCount; ++i)
		{
			std::string vertexPosition;
			file >> vertexPosition;

			positionVertex vert;
			splitVertex(vertexPosition, ',', vert);
			if (vert.x == NONE_FLOAT || vert.y == NONE_FLOAT)
				throw "Wrong positiong of vertex";
			res.m_vecPositionVertex.push_back(vert);
		}

		std::string chipsPosition;
		file >> chipsPosition;
		auto& positionChips = split(chipsPosition, ',', res.m_positionChips);
		if (positionChips.size() != res.m_chipCount)
			throw "Wrong number of chips";

		std::string chipsPositionWin;
		file >> chipsPositionWin;

		auto winPositionChips = split(chipsPositionWin, ',', res.m_winPositionChips);
		if (winPositionChips.size() != res.m_chipCount)
			throw "Wrong number of chips";

		std::string sEdgeCount;
		file >> sEdgeCount;
		int nEdgeCount = std::stoi(sEdgeCount);
		res.m_vecPositionEdge.reserve(nEdgeCount);

		for (int i = 0; i < nEdgeCount; i++)
		{
			std::string edgePosition;
			file >> edgePosition;

			positionEdge edge;
			splitEdge(edgePosition, ',', edge);
			if (edge.s == NONE_INT || edge.f == NONE_INT)
				throw "Wrong edges";

			res.m_vecPositionEdge.push_back(edge);
		}
	}
	catch (const std::exception& ex)
	{
		std::printf("Failed to load configuration: %s,\n file: %s", ex.what(), path);
	}

	file.close();

	return { res };
}
