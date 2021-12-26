#include "Ghost.h"

Ghost::Ghost()
	: m_color{0}
{
}

Ghost::Ghost(sf::Vector2u spriteSize, float speed, int color)
	: m_color{color}
{
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_speed = speed;
}

Ghost::Ghost(sf::Vector2f coords, sf::Vector2u spriteSize, float speed, int direction, sf::Vector2f speedVec, int color)
{
	m_coords = coords;
	m_direction = direction;
	m_nextDirection = -1;
	m_spriteSize = spriteSize;
	m_speed = speed;
	m_color = color;

	updateSpeedVec(speedVec);
	setPosition(m_coords);
}

int Ghost::color()
{
	return m_color;
}

void Ghost::setColor(int color)
{
	if (m_color == color)
		return;

	m_color = color;
}

void Ghost::loadSprite()
{
	m_sprite.setTexture(*m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, m_color * m_spriteSize.y, m_spriteSize.x, m_spriteSize.y));
}

