#include "BaseEntity.h"

bool BaseEntity::loadSprite(const std::string& pathToTheTexture)
{
	if (!m_texture.loadFromFile(pathToTheTexture))
		return false;

	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_spriteSize.x, m_spriteSize.y));

	return true;
}

void BaseEntity::changeTextureRect(sf::IntRect rect)
{
	m_sprite.setTextureRect(rect);
}

//sf::Vector2f BaseEntity::getNewCoords()
//{
//	return m_coords + m_speedVec;
//}

sf::Vector2u BaseEntity::spriteSize()
{
	return m_spriteSize;
}

sf::Vector2f BaseEntity::coords()
{
	return m_coords;
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

void BaseEntity::updateSpeedVec()
{
	switch (m_direction)
	{
	case Direction::RIGHT:
		m_speedVec = sf::Vector2f(1, 0) * m_speed;
		break;
	case Direction::LEFT:
		m_speedVec = sf::Vector2f(-1, 0) * m_speed;
		break;
	case Direction::UP:
		m_speedVec = sf::Vector2f(0, -1) * m_speed;
		break;
	case Direction::DOWN:
		m_speedVec = sf::Vector2f(0, 1) * m_speed;
		break;
	default:
		break;
	}
}

void BaseEntity::moveTo(sf::Vector2f newCoords)
{
	m_coords = newCoords;
	setPosition(m_coords);
}

void BaseEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	states.transform *= getTransform();

	states.texture = &m_texture;

	target.draw(m_sprite, states);
}
