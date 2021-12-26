#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

struct EntityParams
{
    EntityParams() : direction{0} {};

    EntityParams(sf::Vector2f coords, int direction) {
        this->coords = coords;
        this->direction = direction;
    }

    EntityParams& operator = (EntityParams right) {
        coords = right.coords;
        direction = right.direction;
        return *this;
    }

    sf::Vector2f coords;
    int direction;
};

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap(sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height, EntityParams* m_pacmanParams, EntityParams* m_ghostsParams);

    void setTexture(sf::Texture* texture);
    bool load();
    void unload();
    int getTileNumber(sf::Vector2f coords) const;
    int getTileValue(const int &tileNumber) const;  
    sf::Vector2f getTileCoords(int tileNumber);
    sf::VertexArray* vertices();
    unsigned int width();
    unsigned int height();
    int widthInPixels();
    int heightInPixels();
    EntityParams* pacmanParams();
    EntityParams* ghostsParams();

    void setup();

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    int* m_tiles;
    sf::VertexArray m_vertices;
    sf::Texture* m_tileset;
    sf::Vector2u m_tileSize;
    unsigned int m_width;
    unsigned int m_height;

    EntityParams* m_pacmanParams;
    EntityParams* m_ghostsParams;
    
};

