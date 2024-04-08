#include "Edge.h"
#include <cmath>

#define EDGE_COLOR 255, 255, 255


Edge::Edge(const sf::Vector2f& pos_s, const sf::Vector2f& pos_d, float mult)
{
	auto dt = pos_s - pos_d;

	float distance = std::sqrt(dt.x * dt.x + dt.y * dt.y);
	m_spriteSize = { distance, 5 * mult };

	m_sprite.setSize(m_spriteSize);
	m_sprite.setOrigin(m_spriteSize / 2.0f);

	m_sprite.setPosition((pos_s.x + pos_d.x) / 2, (pos_s.y + pos_d.y) / 2);

	m_sprite.setFillColor(sf::Color::Color(EDGE_COLOR));

	float degree = std::atan2(dt.y, dt.x) * 180 / 3.14159f;
	m_sprite.setRotation(degree);
}

Edge::~Edge() {}

void Edge::OnDraw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}