#include "Background.h"

Background::Background() {}

Background::~Background() {}

void Background::SetSprite(const sf::Sprite& sp)
{
	m_sprite = sp;
	m_scale = m_sprite.getScale();
}

void Background::Update(float dt_time)
{
	auto curr = m_sprite.getScale();
	auto val = m_scale - curr;

	if (m_up && val.x < -0.20f)
		m_up = false;

	if (!m_up && val.x > 0.0f)
		m_up = true;

	auto factory = m_speed * dt_time;

	factory = m_up ? factory : -factory;

	m_sprite.setScale(curr.x + factory, curr.y + factory);
}

void Background::OnDraw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}
