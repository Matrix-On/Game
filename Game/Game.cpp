#include "Game.h"

#include <limits>
#include <thread>
#include <chrono>
#include <sstream>

#define PATH_TO_BACKGROUND "Resource/background.jpg"
#define PATH_TO_LEVELS "Resource/levels/"
#define PATH_TO_FONT "Resource/fonts/sansation.ttf"
#define PATH_TO_CHIP "Resource/chip/chip.png"
#define PATH_TO_CHIPBACK "Resource/chip/chip_background.png"

#define GAME_NAME "CUPBOARDS"
#define GAME_OVER "GAME OVER"
#define GAME_LOADING "LOADING..."
#define STEPS "STEPS: 0"
#define LEVEL "LEVEL: 0"
#define PRESS_SPACE_START "PRESS SPACE TO START GAME"
#define PRESS_SPACE_CONTINUE "PRESS SPACE TO CONTINUE GAME"

#define BORDER_LEFT 100.0f
#define BORDER_RIGHT 100.0f
#define BORDER_TOP 100.0f
#define BORDER_BOTTOM 100.0f

#define CONST_MULT 2.2f

extern sf::Color g_colors[];


Game::Game() :
	m_gameState(GameState::MAIN_MENU),
	m_isInit(false),
	m_level(1),
	m_totalSteps(0),
	m_chipSelected(-1),
#ifdef _DEBUG
	m_rendWindow(sf::VideoMode(1200, 800), GAME_NAME, sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 16))
#else
	m_rendWindow(sf::VideoMode(1200, 800), GAME_NAME, sf::Style::Fullscreen, sf::ContextSettings(0, 0, 16))
#endif // _DEBUG
{
	m_rendWindow.setVerticalSyncEnabled(true);
	m_rendWindow.setFramerateLimit(120);
}

Game::~Game()
{

}

bool Game::Init()
{
	if (m_isInit)
		return false;

	if (!LoadResource())
		return false;

	sf::Vector2f windowSize{ (float)m_rendWindow.getSize().x , (float)m_rendWindow.getSize().y };

	if (!LoadBackground(windowSize))
		return false;

	if (!LoadTexts(windowSize))
		return false;

	m_isInit = true;

	return m_isInit;
}

void Game::Run()
{
	if (!m_isInit) return;

	sf::Clock clock;

	m_totalSteps = 0;
	m_chipSelected = -1;

	while (m_rendWindow.isOpen())
	{
		sf::Vector2f pos = m_rendWindow.mapPixelToCoords(sf::Mouse::getPosition(m_rendWindow));

		PollEvent(pos);

		float clockTime = clock.restart().asSeconds();
		m_rendWindow.clear();
		m_background.Update(clockTime);
		m_background.OnDraw(m_rendWindow);

		ProccessGame(clockTime, pos);

		m_rendWindow.display();

		if (m_gameState == GameState::LEVEL_LOADING)
		{
			//loading simulation
			std::this_thread::sleep_for(std::chrono::seconds(1));
			clock.restart();
			m_gameState = GameState::LEVEL_ON;
		}
	}
}

void Game::PollEvent(sf::Vector2f& pos)
{
	sf::Event event;
	while (m_rendWindow.pollEvent(event))
	{
		if (IsClosed(event))
		{
			m_rendWindow.close();
			break;
		}

		if (IsNextLevel(event))
			GameNextLevel();

		if (!IsClicked(event))
			continue;



		int tempSelected = m_chipSelected;
		m_chipSelected = OnChipClicked(pos);

		if (m_chipSelected == -1 && tempSelected != -1
			&& OnVertexClicked(pos, tempSelected))
		{
			++m_totalSteps;
			textes.m_steps.setString("STEPS: " + std::to_string(m_totalSteps));
		}
	}
}

void Game::GameNextLevel()
{
	if (m_gameState == GameState::GAMES_OVER)
	{
		m_gameState = GameState::MAIN_MENU;
		return;
	}
	m_gameState = GameState::LEVEL_LOADING;

	std::ostringstream path;
	path << PATH_TO_LEVELS << m_level << ".txt";
	if (LoadLevel(path.str().c_str()))
	{
		path.str("");
		path << "LEVEL " << m_level;
		textes.m_level.setString(path.str());

		m_totalSteps = 0;
		m_level++;

		path.str("");
		path << m_totalSteps;
		textes.m_steps.setString("STEPS: " + path.str());

		//but simulating level loading
		//m_gameState = GameState::LEVEL_ON;
		return;
	}

	m_totalSteps = 0;
	m_level = 1;
	m_gameState = GameState::GAMES_OVER;
}

void Game::GameMainMenu()
{
	m_rendWindow.draw(textes.m_nameGame);
	m_rendWindow.draw(textes.m_pressStart);
}

void Game::GameLevelLoading()
{
	m_rendWindow.draw(textes.m_loading);
}

void Game::GameLevelComplited(float clockTime)
{
	for (auto& chip : m_chips)
		chip.Update(clockTime);

	for (auto& edge : m_edges)
		edge.OnDraw(m_rendWindow);

	for (auto& vertex : m_vertexes)
		vertex.OnDraw(m_rendWindow);

	for (auto& chip : m_chips)
		chip.OnDraw(m_rendWindow);

	m_rendWindow.draw(textes.m_steps);
	m_rendWindow.draw(textes.m_level);
	m_rendWindow.draw(textes.m_nextLevel);
}

void Game::GameLevelOn(float clockTime, sf::Vector2f& pos)
{
	if (IsWin()) m_gameState = GameState::LEVEL_COMPLETE;

	for (auto& vertex : m_vertexes)
		vertex.OnEntered(pos);

	for (auto& chip : m_chips)
		chip.OnEnetered(pos);

	for (auto& chip : m_chips)
		chip.Update(clockTime);

	for (auto& edge : m_edges)
		edge.OnDraw(m_rendWindow);

	for (auto& vertex : m_vertexes)
		vertex.OnDraw(m_rendWindow);

	for (auto& chip : m_chips)
		chip.OnDraw(m_rendWindow);

	m_rendWindow.draw(textes.m_steps);
	m_rendWindow.draw(textes.m_level);
}

void Game::GameOver()
{
	m_rendWindow.draw(textes.m_gameOver);
	m_rendWindow.draw(textes.m_nextLevel);
}

void Game::ProccessGame(float clockTime, sf::Vector2f& pos)
{
	switch (m_gameState)
	{
	case GameState::MAIN_MENU:
		GameMainMenu();
		break;
	case GameState::LEVEL_LOADING:
		GameLevelLoading();
		break;
	case GameState::LEVEL_COMPLETE:
		GameLevelComplited(clockTime);
		break;
	case GameState::LEVEL_ON:
		GameLevelOn(clockTime, pos);
		break;
	case GameState::GAMES_OVER:
		GameOver();
		break;
	default:
		break;
	}
}

bool Game::LoadBackground(const sf::Vector2f& winSize)
{

	sf::Sprite background;
	background.setTexture(textures.m_Background);
	background.setPosition(0, 0);

	auto backSize = textures.m_Background.getSize();
	float divX = winSize.x / backSize.x;
	float divY = winSize.y / backSize.y;

	float scale_background = std::max(divX, divY);

	background.scale(scale_background, scale_background);
	m_background.SetSprite(background);

	return true;
}

void Game::SetText(sf::Text& text, unsigned int size, const char str[], int charset, float originY, float positionX,  float positionY)
{
	text.setFont(m_font);
	text.setCharacterSize(size);
	text.setString(str);
	text.setFillColor(sf::Color::White);

	if (charset != NONE_INT && originY != NONE_FLOAT)
		text.setOrigin(text.findCharacterPos(charset).x / 2, originY);
	if (positionX != NONE_FLOAT && positionY != NONE_FLOAT)
		text.setPosition(positionX, positionY);
}

bool Game::LoadTexts(const sf::Vector2f& winSize)
{
	float winDiv2X = winSize.x / 2;
	float winDiv2Y = winSize.y / 2;

	SetText(textes.m_nameGame, 100, GAME_NAME,
		9, 100,
		winDiv2X, winDiv2Y);
	SetText(textes.m_gameOver, 100, GAME_OVER,
		9, 100,
		winDiv2X, winDiv2Y);
	SetText(textes.m_pressStart, 40, PRESS_SPACE_START,
		100, 0,
		winDiv2X, winDiv2Y);
	SetText(textes.m_nextLevel, 40, PRESS_SPACE_CONTINUE,
		100, 0,
		winDiv2X, winDiv2Y);
	SetText(textes.m_loading, 40, GAME_LOADING,
		100, 40,
		winDiv2X, winDiv2Y - 20);
	SetText(textes.m_steps, 40, STEPS);
	SetText(textes.m_level, 40, LEVEL,
		100, 0,
		winDiv2X, 0.0f);
	return true;
}

bool Game::LoadResource()
{
	try
	{
		if (!textures.m_chip.loadFromFile(PATH_TO_CHIP))
			throw PATH_TO_CHIP;

		if (!textures.m_chipBackground.loadFromFile(PATH_TO_CHIPBACK))
			throw PATH_TO_CHIPBACK;

		if (!m_font.loadFromFile(PATH_TO_FONT))
			throw PATH_TO_FONT;

		if (!textures.m_Background.loadFromFile(PATH_TO_BACKGROUND))
			throw PATH_TO_BACKGROUND;
	}
	catch (const std::exception& ex)
	{
		printf("Faild to load: %s", ex.what());
		return false;
	}

	return true;
}

bool Game::LoadVertex(configureGame& gameConf, float& mult)
{
	sf::Sprite spriteShipBackground;
	spriteShipBackground.setTexture(textures.m_chipBackground);

	for (auto& pos : gameConf.m_vecPositionVertex)
		m_vertexes.push_back({ { pos.x, pos.y }, mult , spriteShipBackground });

	for (auto& pos : gameConf.m_winPositionChips)
		m_vertexes[pos].SetColor(g_colors[pos]);

	return true;
}

bool Game::LoadChips(configureGame& gameConf, float& mult)
{
	sf::Sprite spriteChip;
	spriteChip.setTexture(textures.m_chip);

	for (size_t i = 0; i < gameConf.m_positionChips.size(); ++i)
	{
		int index = gameConf.m_positionChips[i];
		int indexWin = gameConf.m_winPositionChips[i];

		m_chips.push_back({ {m_vertexes[index].GetPosition().x ,m_vertexes[index].GetPosition().y },
			mult, spriteChip, m_vertexes[indexWin].GetColor() });

		m_chips[m_chips.size() - 1].SetVertex(index);
		m_chips[m_chips.size() - 1].SetDestVertex(indexWin);

		m_gameGraph.SetChip(index, true);
	}

	return true;
}

bool Game::LoadEdges(configureGame& gameConf, float& mult)
{
	for (auto& pos : gameConf.m_vecPositionEdge)
	{
		m_gameGraph.AddEdge(pos.f, pos.s);

		m_edges.push_back({
			{m_vertexes[pos.f].GetPosition().x ,m_vertexes[pos.f].GetPosition().y},
			{m_vertexes[pos.s].GetPosition().x ,m_vertexes[pos.s].GetPosition().y},
			mult });
	}

	return true;
}

bool Game::LoadLevel(const char* pathLevel)
{
	m_vertexes.clear();
	m_chips.clear();
	m_edges.clear();

	Config config;
	auto optionalConf = config.Parse(pathLevel);
	if (!optionalConf) return false;

	configureGame& gameConf = optionalConf.value();

	m_gameGraph.SetCountVertex(gameConf.m_vertexCount);

	auto mult = SetScale(gameConf, m_rendWindow.getSize());
	if (mult > CONST_MULT) mult = CONST_MULT;

	m_vertexes.reserve(gameConf.m_vertexCount);
	m_chips.reserve(gameConf.m_chipCount);
	m_edges.reserve(gameConf.m_vecPositionEdge.size());

	LoadVertex(gameConf, mult);
	LoadChips(gameConf, mult);
	LoadEdges(gameConf, mult);

	return true;
}

bool Game::IsWin()
{
	for (auto& chip : m_chips)
	{
		if (chip.GetDestVertex() != chip.GetVertex()) return false;
	}

	return true;
}

bool Game::IsClosed(sf::Event& event)
{
	if (event.type == sf::Event::Closed ||
		(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
		return true;
	return false;
}

bool Game::IsNextLevel(sf::Event& event)
{
	if ((m_gameState == GameState::MAIN_MENU ||
		m_gameState == GameState::LEVEL_COMPLETE ||
		m_gameState == GameState::GAMES_OVER)
		&& event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		return true;
	return false;
}

bool Game::IsClicked(sf::Event& event)
{
	if (m_gameState == GameState::LEVEL_ON &&
		event.type == sf::Event::MouseButtonPressed &&
		event.key.code == sf::Mouse::Left)
		return true;
	return false;
}

float Game::SetScale(configureGame& config, const sf::Vector2u& window_size)
{
	float widthScreen = window_size.x - (BORDER_LEFT + BORDER_RIGHT);
	float hightScreen = window_size.y - (BORDER_TOP + BORDER_BOTTOM);

	float maxX = 0.f;
	float maxY = 0.f;

	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();

	for (auto& pos : config.m_vecPositionVertex)
	{
		if (pos.x > maxX) maxX = pos.x;
		if (pos.y > maxY) maxY = pos.y;
		if (pos.x < minX) minX = pos.x;
		if (pos.y < minY) minY = pos.y;
	}


	float dX = maxX - minX;
	float dY = maxY - minY;
	float multX = widthScreen / dX;
	float multY = hightScreen / dY;

	float mult = std::min(multX, multY);
	float offsetX = (widthScreen - mult * dX) / 2.0f;
	float offsetY = (hightScreen - mult * dY) / 2.0f;

	for (auto& pos : config.m_vecPositionVertex)
	{
		pos.x = (pos.x - minX) * mult + BORDER_LEFT + offsetX;
		pos.y = (pos.y - minY) * mult + BORDER_TOP + offsetY;
	}

	return mult;
}

int Game::OnChipClicked(const sf::Vector2f& pos)
{
	int index = -1;
	for (size_t i = 0; i < m_chips.size(); i++)
	{
		if (m_chips[i].OnSelected(pos))
		{
			index = (int)i;
			auto dest = m_gameGraph.GetDest(m_chips[index].GetVertex());
			for (size_t j = 0; j < dest.size(); j++)
				m_vertexes[dest[j]].SetFlash(true);
		}
	}

	return index;
}

bool Game::OnVertexClicked(const sf::Vector2f& pos, int chip_index)
{
	bool moved = false;
	for (size_t i = 0; i < m_vertexes.size(); i++)
	{
		m_vertexes[i].SetFlash(false);
		if (m_vertexes[i].OnSelected(pos))
		{
			auto v_path = m_gameGraph.GetPath(m_chips[chip_index].GetVertex(), (int)i);
			if (v_path.empty())
				continue;

			std::vector<sf::Vector2f> path;
			for (size_t j = 0; j < v_path.size(); j++)
				path.push_back(m_vertexes[v_path[j]].GetPosition());

			m_gameGraph.SetChip(m_chips[chip_index].GetVertex(), false);
			m_gameGraph.SetChip((int)i, true);

			m_chips[chip_index].MoveTo(path);
			m_chips[chip_index].SetVertex((int)i);
			moved = true;
		}
	}
	return moved;
}
