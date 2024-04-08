#include "Chip.h"
#include "../Operations/Operations.h"

#define VERTEX_SIZEX 40.0f
#define VERTEX_SIZEY 34.3f

#define BACKGROUND_SIZEX 46.0f
#define BACKGROUND_SIZEY 39.4f

#define SPEED 400.0f

#define DISTANSION 10.0f

//#define BACKGROUND_COLOR 255
#define BACKGROUND_COLOR_SELECTED 90
#define BACKGROUND_COLOR_STANDART 0
//#define VERTEX_COLOR_SELECTED 90
#define VERTEX_COLOR_STANDART 230

Chip::Chip(const sf::Vector2f& pos, float mult, const sf::Sprite& sp, sf::Color color) :
	m_spriteVertex(sp),
	m_spriteBackground(sp),
	m_spriteVertexSize(VERTEX_SIZEX* mult, VERTEX_SIZEY* mult),
	m_spriteBackgroundSize(BACKGROUND_SIZEX* mult, BACKGROUND_SIZEY* mult),
	m_speed(SPEED* mult),
	m_moveTo(false),
	m_isSelected(false),
	m_vertex(-1),
	m_DestVertex(-1)
{
	m_spriteVertex.setScale(m_spriteVertexSize.x / sp.getTexture()->getSize().x,
		m_spriteVertexSize.y / sp.getTexture()->getSize().y);
	m_spriteVertex.setOrigin(sp.getTexture()->getSize().x / 2.f,
		sp.getTexture()->getSize().y / 2.f);
	m_spriteVertex.setPosition(pos);

	m_spriteBackground.setScale(m_spriteBackgroundSize.x / sp.getTexture()->getSize().x,
		m_spriteBackgroundSize.y / sp.getTexture()->getSize().y);
	m_spriteBackground.setOrigin(sp.getTexture()->getSize().x / 2.f, sp.getTexture()->getSize().y / 2.f);
	m_spriteBackground.setPosition(pos);

	m_colorVertexSelected = color;
	m_colorVertexStandart = color;
	m_colorVertexStandart.a = 230;
	m_spriteVertex.setColor(m_colorVertexStandart);

	m_colorBackgroundSelected = color;
	m_colorBackgroundSelected.a = 90;
	m_colorBackgroundStandart = color;
	m_colorBackgroundStandart.a = 0;
	m_spriteBackground.setColor(m_colorBackgroundStandart);
}

Chip::~Chip() {}

void Chip::MoveTo(const std::vector<sf::Vector2f>& path)
{
	m_moveTo = true;

	for (auto& elem : path)
		m_path2Dest.push(elem);
}

bool Chip::OnSelected(const sf::Vector2f& pos)
{
	m_isSelected = Operations::CheckInClickPos(m_spriteVertexSize, m_spriteVertex.getPosition(), pos);
	m_spriteVertex.setColor(m_isSelected ? m_colorVertexSelected : m_colorVertexStandart);

	return m_isSelected;
}

void Chip::OnEnetered(const sf::Vector2f& pos)
{
	if (Operations::CheckInClickPos(m_spriteVertexSize, m_spriteVertex.getPosition(), pos))
	{
		m_spriteBackground.setColor(m_colorBackgroundSelected);
	}
	else if (!m_isSelected)
	{
		m_spriteBackground.setColor(m_colorBackgroundStandart);
	}
}

void Chip::Update(float deltaTime)
{
	if (!m_moveTo)
		return;

	auto pos = m_path2Dest.front();
	float dx = pos.x - m_spriteVertex.getPosition().x;
	float dy = pos.y - m_spriteVertex.getPosition().y;
	float distance = std::sqrt(dx * dx + dy * dy);

	if (distance > DISTANSION)
	{
		float alpha = std::atan2(dy, dx);

		float deltaSpeed = m_speed * deltaTime;
		float offsetX = deltaSpeed * std::cos(alpha);
		float offsetY = deltaSpeed * std::sin(alpha);

		m_spriteVertex.move(offsetX, offsetY);
		m_spriteVertex.move(offsetX, offsetY);
		return;
	}

	m_path2Dest.pop();

	if (m_path2Dest.empty())
		m_moveTo = false;

	m_spriteVertex.setPosition(pos.x, pos.y);
	m_spriteBackground.setPosition(pos.x, pos.y);

}

void Chip::OnDraw(sf::RenderWindow& window)
{
	window.draw(m_spriteBackground);
	window.draw(m_spriteVertex);
}
