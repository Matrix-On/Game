#include "Vertex.h"
#include "../Operations/Operations.h"

#define VERTEX_SIZEX 70.0f 
#define VERTEX_SIZEY 60.0f

#define BACKGROUND_SIZEX 76.0f
#define BACKGROUND_SIZEY 65.1f

#define BACKGROUND_COLOR 255
#define BACKGROUND_COLOR_SELECTED 100
#define BACKGROUND_COLOR_STANDART 0

Vertex::Vertex(const sf::Vector2f& pos, float mult, const sf::Sprite& sp, sf::Color color)
	:
	m_spriteVertex(sp),
	m_spriteVertexSize(VERTEX_SIZEX* mult, VERTEX_SIZEY* mult),
	m_spriteBackground(sp),
	m_spriteBackgroundSize(BACKGROUND_SIZEX* mult, BACKGROUND_SIZEY* mult),
	m_isSelected(false),
	m_inPlay(false),
	m_isFlash(false)
{
	m_spriteVertex.setScale(m_spriteVertexSize.x / sp.getTexture()->getSize().x,
		m_spriteVertexSize.y / sp.getTexture()->getSize().y);

	m_spriteVertex.setOrigin(sp.getTexture()->getSize().x / 2.f,
		sp.getTexture()->getSize().y / 2.f);
	m_spriteVertex.setPosition(pos.x, pos.y);

	m_spriteBackground.setScale(m_spriteBackgroundSize.x / sp.getTexture()->getSize().x,
		m_spriteBackgroundSize.y / sp.getTexture()->getSize().y);

	m_spriteBackground.setOrigin(sp.getTexture()->getSize().x / 2.f,
		sp.getTexture()->getSize().y / 2.f);
	m_spriteBackground.setPosition(pos.x, pos.y);

	SetColor(color);
}

Vertex::~Vertex() {}

void Vertex::SetColor(sf::Color color)
{
	m_colorVertexSelected = color;
	m_colorVertexStandart = color == sf::Color::White ? sf::Color::Transparent : color;
	m_spriteVertex.setColor(m_colorVertexStandart);

	m_colorBackgroundSelected = color;
	m_colorBackgroundSelected.a = BACKGROUND_COLOR_SELECTED;

	m_colorBackgroundStandart = color == sf::Color::White ? sf::Color::Transparent : color;
	m_colorBackgroundStandart.a = BACKGROUND_COLOR_STANDART;
	m_spriteBackground.setColor(m_colorBackgroundStandart);
}

sf::Color Vertex::GetColor()
{
	auto col = m_colorVertexStandart;
	col.a = BACKGROUND_COLOR;
	return col;
}

bool Vertex::OnSelected(const sf::Vector2f& pos)
{
	m_isSelected = Operations::CheckInClickPos(m_spriteVertexSize, m_spriteVertex.getPosition(), pos);
	return m_isSelected;
}

void Vertex::OnEntered(const sf::Vector2f& pos)
{
	if (Operations::CheckInClickPos(m_spriteVertexSize, m_spriteVertex.getPosition(), pos))
	{
		m_spriteBackground.setColor(m_colorBackgroundSelected);
		m_inPlay = true;
	}
	else
	{
		if (!m_isFlash)
			m_spriteBackground.setColor(m_colorBackgroundStandart);
		m_inPlay = false;
	}
}

void Vertex::SetFlash(bool isFlash)
{
	m_isFlash = isFlash;
	m_spriteBackground.setColor(m_isFlash ? m_colorBackgroundSelected : m_colorBackgroundStandart);
}

void Vertex::OnDraw(sf::RenderWindow& window)
{
	window.draw(m_spriteBackground);
	window.draw(m_spriteVertex);
}
