#pragma once
#include "build.h"
#include <SFML/Graphics.hpp>
#include <queue>

class Chip
{
public:
	Chip(const sf::Vector2f& pos, float mult, const sf::Sprite& sp, sf::Color color = sf::Color::Transparent);
	~Chip();


	bool OnSelected(const sf::Vector2f& pos);
	void OnEnetered(const sf::Vector2f& pos);
	void OnDraw(sf::RenderWindow& window);


	void MoveTo(const std::vector<sf::Vector2f>& path);

	inline const sf::Vector2f& GetPosition() { return m_spriteVertex.getPosition(); }
	inline void SetVertex(int vertex) { m_vertex = vertex; }
	inline int GetVertex() { return m_vertex; }
	inline void SetDestVertex(int destVertex) { m_DestVertex = destVertex; }
	inline int GetDestVertex() { return m_DestVertex; }

	void Update(float dt_time);
private:
	float m_speed = 0.f;

	bool m_moveTo;
	bool m_isSelected;

	int m_vertex;
	int m_DestVertex;
	
	std::queue<sf::Vector2f> m_path2Dest;

	sf::Sprite m_spriteVertex;
	sf::Sprite m_spriteBackground;

	sf::Color m_colorVertexStandart;
	sf::Color m_colorBackgroundStandart;
	sf::Color m_colorVertexSelected;
	sf::Color m_colorBackgroundSelected;

	sf::Vector2f m_spriteBackgroundSize;
	sf::Vector2f m_spriteVertexSize;
};
