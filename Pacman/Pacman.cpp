#include "Pacman.h"

Pacman::Pacman()
	: score{ 0 }
	, m_isStandStill{ true }
	, m_maxScore{ 0 }
{

}

Pacman::Pacman(sf::Vector2u spriteSize, float speed)
	: score{ 0 }
	, m_isStandStill{ true }
	, m_maxScore{ 0 }
{
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_speed = speed;
}

Pacman::Pacman(sf::Vector2f coords, sf::Vector2u spriteSize, float speed, int direction, sf::Vector2f speedVec)
	: score{ 0 }
	, m_isStandStill{ true }
	, m_maxScore{ 0 }
{
	m_coords = coords;
	m_direction = direction;
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_speed = speed;
	
	updateSpeedVec(speedVec);
	setPosition(m_coords);
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

void Pacman::loadSprite()
{
	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_spriteSize.x, m_spriteSize.y));
}

int Pacman::nextDirection()
{
	return m_nextDirection;
}

