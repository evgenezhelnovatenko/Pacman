#include "TileMap.h"
#include <iostream>

const int TileMap::DOTTED_CELL;
const int TileMap::EMPTY_CELL;
const int TileMap::PACMAN_CELL;
const int TileMap::GHOST_CELL;

TileMap::TileMap(sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height, int ghostCount)
    : m_tiles{ tiles }
    , m_tileset{ nullptr }
    , m_tileSize{ tileSize }
    , m_width{ width }
    , m_height{ height }
    , m_pacmanParams{ new EntityParams() }
    , m_ghostsParams{ new EntityParams[ghostCount] }
{
}

TileMap::TileMap(sf::Vector2u tileSize, int* tiles, unsigned int width, unsigned int height, int ghostCount, EntityParams* pacmanParams, EntityParams* ghostsParams)
    : TileMap (tileSize, tiles, width, height, ghostCount)
{
    m_pacmanParams = pacmanParams;
    m_ghostsParams = ghostsParams;
}

void TileMap::setTexture(sf::Texture* texture)
{
    if (m_tileset == texture)
        return;

    m_tileset = texture;
}


bool TileMap::load()
{ 
    // resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(m_width * m_height * 4);

    populateTheVertexArray();

    return true;
}

// Наполнение массива вертексов.
void TileMap::populateTheVertexArray()
{
    int ghostIndex = -1;
    for (unsigned int i = 0; i < m_width; ++i)
        for (unsigned int j = 0; j < m_height; ++j)
        {
            // get the current tile number
            int tileIndex = i + j * m_width;
            int tileNumber = m_tiles[tileIndex];

            if (tileNumber == PACMAN_CELL) {
                m_pacmanParams->coords = getTileCoords(tileIndex);
                m_pacmanParams->direction = Direction::RIGHT;
                replacingTileNumber(tileIndex, EMPTY_CELL);
            }
            else if (tileNumber == GHOST_CELL) {
                ghostIndex++;
                m_ghostsParams[ghostIndex].coords = getTileCoords(tileIndex);
                m_ghostsParams[ghostIndex].direction = ghostIndex;
                replacingTileNumber(tileIndex, EMPTY_CELL);
            }

            // find its position in the tileset texture
            int tu = tileNumber % (m_tileset->getSize().x / m_tileSize.x);
            int tv = tileNumber / (m_tileset->getSize().x / m_tileSize.x);

            // get a pointer to the current tile's quad
            sf::Vertex* quad = &m_vertices[tileIndex * 4];

            // define its 4 corners
            quad[0].position = sf::Vector2f(i * m_tileSize.x, j * m_tileSize.y);
            quad[1].position = sf::Vector2f((i + 1) * m_tileSize.x, j * m_tileSize.y);
            quad[2].position = sf::Vector2f((i + 1) * m_tileSize.x, (j + 1) * m_tileSize.y);
            quad[3].position = sf::Vector2f(i * m_tileSize.x, (j + 1) * m_tileSize.y);

            // define its 4 texture coordinates
            quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
            quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
            quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
            quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);
        }
}

void TileMap::unload()
{
    m_vertices.clear();
    delete m_tileset;
    m_tileset = nullptr;
}

void TileMap::updateVertexArray()
{
    m_vertices.clear();
    m_vertices.resize(m_width * m_height * 4);
    populateTheVertexArray();
}

int TileMap::getTileIndex(sf::Vector2f coords) const
{
    int i = coords.y / m_tileSize.y;
    int j = coords.x / m_tileSize.x;


    return i * m_width + j;
}

int TileMap::getTileNumber(const int &tileIndex) const
{
    if (tileIndex < 0 || tileIndex >= m_width * m_height) {
        std::cout << "Going beyond the boundaries of the map ";
        return -1;
    }

    return m_tiles[tileIndex];
}

sf::Vector2f TileMap::getTileCoords(int tileIndex)
{
    int i = tileIndex / m_width;
    int j = tileIndex % m_width;

    return sf::Vector2f(j * m_tileSize.x, i * m_tileSize.y);
}

void TileMap::replacingTileNumber(int tileIndex, int newTileNumber)
{
    m_tiles[tileIndex] = newTileNumber;
}

sf::VertexArray* TileMap::vertices()
{
    return &m_vertices;
}

unsigned int TileMap::width()
{
    return m_width;
}

unsigned int TileMap::height()
{
    return m_height;
}

int TileMap::widthInPixels()
{
    return m_width * m_tileSize.x;
}

int TileMap::heightInPixels()
{
    return m_height * m_tileSize.y;
}

EntityParams* TileMap::pacmanParams()
{
    return m_pacmanParams;
}

EntityParams* TileMap::ghostsParams()
{
    return m_ghostsParams;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_tileset == nullptr || m_vertices.getVertexCount() == 0)
        return;

    // apply the transform
    states.transform *= getTransform();

    // apply the tileset texture
    states.texture = m_tileset;

    // draw the vertex array
    target.draw(m_vertices, states);
}
