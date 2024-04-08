#include "Operations.h"

bool Operations::CheckInClickPos(const sf::Vector2f& size, const sf::Vector2f& posStart, const sf::Vector2f& posDest)
{
	auto sizeDiv2X = size.x / 2;
	auto sizeDiv2Y = size.y / 2;

	auto Minus_SpriteX_SizeDiv2X = posStart.x - sizeDiv2X;
	auto Plus_SpriteX_SizeDiv2X = posStart.x + sizeDiv2X;

	auto Minus_SpriteY_SizeDiv2Y = posStart.y - sizeDiv2Y;
	auto Plus_spriteY_SizeDiv2Y = posStart.y + sizeDiv2Y;

	if ((Minus_SpriteX_SizeDiv2X < posDest.x && Plus_SpriteX_SizeDiv2X > posDest.x)
		&& (Minus_SpriteY_SizeDiv2Y < posDest.y && Plus_spriteY_SizeDiv2Y > posDest.y))
	{
		return true;
	}

	return false;
}
