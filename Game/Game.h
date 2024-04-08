#pragma once
#include <SFML/Graphics.hpp>

#include "Constants/Constants.h"
#include "Config/Config.h"
#include "Elements/Graph.h"
#include "Elements/Chip.h"
#include "Elements/Vertex.h"
#include "Elements/Edge.h"
#include "Elements/Background.h"


class Game
{
public:
	Game();
	~Game();

	bool Init();
	void Run();

protected:
	bool LoadResource();
	bool LoadLevel(const char* pathLevel);

	void ProccessGame(float clockTime, sf::Vector2f& pos);
	void GameMainMenu();
	void GameLevelLoading();
	void GameLevelOn(float clockTime, sf::Vector2f& pos);
	void GameLevelComplited(float clockTime);
	void GameOver();
	void GameNextLevel();

	void PollEvent(sf::Vector2f& pos);
	float SetScale(configureGame& config, const sf::Vector2u& window_size);
	int OnChipClicked(const sf::Vector2f& pos);
	bool OnVertexClicked(const sf::Vector2f& pos, int chip_index);

	bool IsWin();
	bool IsClosed(sf::Event& event);
	bool IsNextLevel(sf::Event& event);
	bool IsClicked(sf::Event& event);

	bool LoadTexts(const sf::Vector2f& winSize);
	bool LoadBackground(const sf::Vector2f& winSize);
	bool LoadVertex(configureGame& gameConf, float& mult);
	bool LoadChips(configureGame& gameConf, float& mult);
	bool LoadEdges(configureGame& gameConf, float& mult);

	void SetText(sf::Text& text, unsigned int size, const char str[],
		int charset = NONE_INT, float originY = NONE_FLOAT, float positionX = NONE_FLOAT, float positionY = NONE_FLOAT);

private:

	sf::RenderWindow m_rendWindow;
	sf::Font m_font;

	struct gameTextures {
		sf::Texture m_chip;
		sf::Texture m_chipBackground;
		sf::Texture m_chipBackgroundBlure;
		sf::Texture m_Background;

	};

	struct gameTextes {
		sf::Text m_nameGame;
		sf::Text m_steps;
		sf::Text m_pressStart;
		sf::Text m_loading;
		sf::Text m_nextLevel;
		sf::Text m_level;
		sf::Text m_gameOver;
	};

	gameTextures textures;
	gameTextes textes;

	Graph m_gameGraph;
	std::vector<Vertex> m_vertexes;
	std::vector<Chip> m_chips;
	std::vector<Edge> m_edges;
	Background m_background;

	GameState m_gameState;
	bool m_isInit;
	int m_chipSelected;
	int m_level;
	int m_totalSteps;
};
