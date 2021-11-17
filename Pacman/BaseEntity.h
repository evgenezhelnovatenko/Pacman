#pragma once

#include "SFML/Graphics.hpp"
#include "Direction.h"
#include "TileMap.h"

class BaseEntity : public sf::Drawable, public sf::Transformable
{

public:
	bool loadSprite(const std::string& pathToTheTexture);
	void changeTextureRect(sf::IntRect rect);
	//sf::Vector2f getNewCoords();
	sf::Vector2u spriteSize();
	sf::Vector2f coords();
	int direction();
	void setDirection(int direction);
	float speed();
	void setSpeed(float speed);
	sf::Vector2f speedVec();
	void updateSpeedVec();

	void moveTo(sf::Vector2f newCoords);

protected:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::Vector2u m_spriteSize;
	sf::Vector2f m_coords;
	float m_speed;
	sf::Vector2f m_speedVec;
	int m_direction;
};

