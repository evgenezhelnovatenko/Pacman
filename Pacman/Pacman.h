#pragma once
#include "BaseEntity.h"

class Pacman : public BaseEntity
{

public:
	Pacman();
	Pacman(sf::Vector2f coords, sf::Vector2u spriteSize, int speed, int direction, sf::Vector2f speedVec);

	bool isStandStill();
	void setIsStandStill(bool isStandStill);
	int nextDirection();
	void setNextDirection(int nextDirection);
	void updateTheCurrentDirectionToTheNext();

private:
	bool m_isStandStill;
	int m_nextDirection;
};

