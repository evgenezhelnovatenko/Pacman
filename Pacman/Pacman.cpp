#include "Pacman.h"

Pacman::Pacman()
{
}

Pacman::Pacman(sf::Vector2f coords, sf::Vector2u spriteSize, int speed, int direction, sf::Vector2f speedVec)
{
	m_coords = coords;
	m_direction = direction;
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_isStandStill = true;
	
	setSpeed(speed);
	updateSpeedVec();
	setPosition(coords);
}

bool Pacman::isStandStill()
{
	return m_isStandStill;
}

void Pacman::setIsStandStill(bool isStandStill)
{
	if (m_isStandStill == isStandStill)
		return;

	m_isStandStill = isStandStill;
}

void Pacman::setNextDirection(int nextDirection)
{
	if (m_nextDirection == nextDirection)
		return;

	m_nextDirection = nextDirection;
}

void Pacman::updateTheCurrentDirectionToTheNext()
{
	m_direction = m_nextDirection;
	m_nextDirection = -1;
}

int Pacman::nextDirection()
{
	return m_nextDirection;
}

