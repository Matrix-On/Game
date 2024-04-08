#pragma once
#include "build.h"
#include <SFML/Graphics.hpp>

class Chip;

class Vertex
{
public:
	Vertex(const sf::Vector2f& pos, float mult, const sf::Sprite& sp, sf::Color color = sf::Color::White);
	~Vertex();

	void SetColor(sf::Color color);
	sf::Color GetColor();

	bool OnSelected(const sf::Vector2f& pos);
	void OnEntered(const sf::Vector2f& pos);
	void OnDraw(sf::RenderWindow& window);

	void SetFlash(bool is_flash);
	inline const sf::Vector2f& GetPosition() { return m_spriteVertex.getPosition(); }
private:

	sf::Sprite m_spriteVertex;
	sf::Sprite m_spriteBackground;

	sf::Color m_colorVertexStandart;
	sf::Color m_colorBackgroundStandart;
	sf::Color m_colorVertexSelected;
	sf::Color m_colorBackgroundSelected;

	sf::Vector2f m_spriteBackgroundSize;
	sf::Vector2f m_spriteVertexSize;

	bool m_inPlay;

	bool m_isSelected;
	bool m_isFlash;
};
