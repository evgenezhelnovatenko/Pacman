#include "BaseEntity.h"

void BaseEntity::changeTextureRect(sf::IntRect rect)
{
	m_sprite.setTextureRect(rect);
}

void BaseEntity::setTexture(sf::Texture* texture)
{
	if (m_texture == texture)
		return;

	m_texture = texture;
}

sf::Vector2u BaseEntity::spriteSize()
{
	return m_spriteSize;
}

sf::Vector2f BaseEntity::coords()
{
	return m_coords;
}

void BaseEntity::setCoords(sf::Vector2f coords)
{
	if (m_coords == coords)
		return;

	m_coords = coords;

	setPosition(m_coords);
}

int BaseEntity::direction()
{
	return m_direction;
}

void BaseEntity::setDirection(int direction)
{
	if (m_direction == direction)
		return;

	m_direction = direction;
}

float BaseEntity::speed()
{
	return m_speed;
}

void BaseEntity::setSpeed(float speed)
{
	if (m_speed == speed)
		return;

	m_speed = speed;
}

sf::Vector2f BaseEntity::speedVec()
{
	return m_speedVec;
}

void BaseEntity::updateSpeedVec(sf::Vector2f speedVec)
{
	if (m_speedVec == speedVec)
		return;

	m_speedVec = speedVec;
}


void BaseEntity::moveTo(sf::Vector2f newCoords)
{
	m_coords = newCoords;
	setPosition(m_coords);
}

void BaseEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	states.transform *= getTransform();

	states.texture = m_texture;

	target.draw(m_sprite, states);
}
