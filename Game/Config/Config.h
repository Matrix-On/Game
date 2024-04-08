#pragma once
#include <optional>
#include <vector>
#include <string>

#define NONE_FLOAT -10000000.0f
#define NONE_INT -10000000

struct positionVertex
{
	float x = NONE_FLOAT;
	float y = NONE_FLOAT;
};

struct positionEdge
{
	int f = NONE_INT;
	int s = NONE_INT;
};

struct configureGame
{
	int m_chipCount;
	int m_vertexCount;
	std::vector<positionVertex> m_vecPositionVertex;
	std::vector<int> m_positionChips;
	std::vector<int> m_winPositionChips;
	std::vector<positionEdge> m_vecPositionEdge;
};

class Config
{
public:
	Config();
	~Config();

	std::optional<configureGame> Parse(const char* path);
private:
	std::vector<int>& split(const std::string& s, char delimer, std::vector<int>& elems);
	positionVertex& splitVertex(const std::string& s, char delimer, positionVertex& vertex);
	positionEdge& splitEdge(const std::string& s, char delimer, positionEdge& edge);
};
