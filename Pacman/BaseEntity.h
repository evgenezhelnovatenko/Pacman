#pragma once

#include "SFML/Graphics.hpp"
#include "Direction.h"
#include "TileMap.h"

class BaseEntity : public sf::Drawable, public sf::Transformable
{

public:
	void loadSprite();
	void changeTextureRect(sf::IntRect rect);
	void setTexture(sf::Texture* texture);
	//sf::Vector2f getNewCoords();
	sf::Vector2u spriteSize();
	sf::Vector2f coords();
	void setCoords(sf::Vector2f coords);
	int direction();
	void setDirection(int direction);
	float speed();
	void setSpeed(float speed);
	sf::Vector2f speedVec();
	void updateSpeedVec(sf::Vector2f speedVec);

	void moveTo(sf::Vector2f newCoords);

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	sf::Sprite m_sprite;
	sf::Texture* m_texture;
	sf::Vector2u m_spriteSize;
	sf::Vector2f m_coords;
	float m_speed;
	sf::Vector2f m_speedVec;
	int m_direction;
	int m_nextDirection;
};

