#pragma once
#include "build.h"
#include <SFML/Graphics.hpp>

class Operations
{
public:
	static bool CheckInClickPos(const sf::Vector2f& size, const sf::Vector2f& posStart, const sf::Vector2f& posDest);
};

