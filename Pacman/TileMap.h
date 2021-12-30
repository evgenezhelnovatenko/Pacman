#pragma once

#include <vector>
#include "SFML/Graphics.hpp"
#include "Enums.h"

struct EntityParams
{
    EntityParams() : direction{-1} {};

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
    TileMap(sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height, int ghostCount);
    TileMap(sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height, int ghostCount, EntityParams* m_pacmanParams, EntityParams* m_ghostsParams);

    void setTexture(sf::Texture* texture);
    bool load();
    void populateTheVertexArray();
    void unload();
    void updateVertexArray();
    int getTileIndex(sf::Vector2f coords) const;
    int getTileNumber(const int &tileIndex) const;  
    sf::Vector2f getTileCoords(int tileIndex);
    void replacingTileNumber(int tileIndex, int newTileNumber);
    sf::VertexArray* vertices();
    unsigned int width();
    unsigned int height();
    int widthInPixels();
    int heightInPixels();
    EntityParams* pacmanParams();
    EntityParams* ghostsParams();

    static const int DOTTED_CELL{ 46 };
    static const int EMPTY_CELL{ 47 };
    static const int PACMAN_CELL{ 50 };
    static const int GHOST_CELL{ 51 };


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

