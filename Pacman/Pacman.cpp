#include "Pacman.h"

Pacman::Pacman()
	: m_isStandStill{ true }
{

}

Pacman::Pacman(sf::Vector2u spriteSize, float speed)
	: m_isStandStill{ true }
{
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_speed = speed;
}

Pacman::Pacman(sf::Vector2f coords, sf::Vector2u spriteSize, float speed, int direction, sf::Vector2f speedVec)
	: m_isStandStill{true}
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

