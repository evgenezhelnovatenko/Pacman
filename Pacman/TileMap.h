#pragma once

#include <vector>
#include "SFML/Graphics.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
    TileMap(sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height);

    bool load(const std::string& tileset);
    int getTileNumber(sf::Vector2f coords) const;
    int getTileValue(const int &tileNumber) const;
    sf::Vector2f getTileCoords(int tileNumber);


private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    int* m_tiles;
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    sf::Vector2u m_tileSize;
    unsigned int m_width;
    unsigned int m_height;
};

