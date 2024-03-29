#pragma once
#include "BaseEntity.h"

class Game;

class Pacman : public BaseEntity
{

public:
	Pacman();
	Pacman(sf::Vector2u spriteSize, float speed);
	Pacman(sf::Vector2f coords, sf::Vector2u spriteSize, float speed, int direction, sf::Vector2f speedVec);

	int score;

	bool isStandStill();
	void setIsStandStill(bool isStandStill);
	int nextDirection();
	void setNextDirection(int nextDirection);
	void updateTheCurrentDirectionToTheNext();

	// Inherited via BaseEntity
	void loadSprite() override;

private:
	bool m_isStandStill;
	
	int m_maxScore;
};

