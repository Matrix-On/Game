#pragma once
#include "build.h"
#include <SFML/Graphics.hpp>


class Edge
{
public:
	Edge(const sf::Vector2f& pos_s, const sf::Vector2f& pos_d, float mult);
	~Edge();
	void OnDraw(sf::RenderWindow& window);
private:
	sf::RectangleShape m_sprite;
	sf::Vector2f m_spriteSize;
};