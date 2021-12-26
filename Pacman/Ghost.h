#pragma once
#include "BaseEntity.h"

class Ghost : public BaseEntity
{
public:
    Ghost();
    Ghost(sf::Vector2u spriteSize, float speed, int color);
    Ghost(sf::Vector2f coords, sf::Vector2u spriteSize, float speed, int direction, sf::Vector2f speedVec, int color);

    int color();
    void setColor(int color);

    // Inherited via BaseEntity
    virtual void loadSprite() override;

private:

    int m_color;

};

