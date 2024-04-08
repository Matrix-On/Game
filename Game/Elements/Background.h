#pragma once
#include "build.h"
#include <SFML/Graphics.hpp>

class Background
{
public:
	Background();
	~Background();

	void OnDraw(sf::RenderWindow& window);
	void SetSprite(const sf::Sprite& sp);
	void Update(float dt_time);

private:
	float m_speed = 0.01f;
	sf::Vector2f m_scale;
	bool m_up = true;
	sf::Sprite m_sprite;
};

