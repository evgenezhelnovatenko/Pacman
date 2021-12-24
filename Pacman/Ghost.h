#pragma once
#include "BaseEntity.h"

class Ghost : public BaseEntity
{
public:
    Ghost();
    Ghost(sf::Vector2u spriteSize, float speed);
    Ghost(sf::Vector2f coords, sf::Vector2u spriteSize, float speed, int direction, sf::Vector2f speedVec);

};

