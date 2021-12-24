#include "Ghost.h"

Ghost::Ghost()
{
}

Ghost::Ghost(sf::Vector2u spriteSize, float speed)
{
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_speed = speed;
}

Ghost::Ghost(sf::Vector2f coords, sf::Vector2u spriteSize, float speed, int direction, sf::Vector2f speedVec)
{
	m_coords = coords;
	m_direction = direction;
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_speed = speed;

	updateSpeedVec(speedVec);
	setPosition(m_coords);
}