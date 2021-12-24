#pragma once
#include <vector>
#include <iostream>

#include "TileMap.h"
#include "Pacman.h"
#include "Ghost.h"


#define MOVE_SPEED 100.0
#define ANIMATION_SPEED 6
#define SPRITE_WIDTH 30
#define SPRITE_HEIGHT 30


class Game
{
public:
	Game(std::string mapTileset, std::string pacmanSpriteSet, std::string ghostsSpriteSet);
	~Game();

	bool setup();
	void levelPreparation(int levelNumb);
	
	void addLevel(TileMap* newLevel);
	TileMap* currentLevel();
	void goToTheNextLevel();
	void goToThePreviousLevel();
	void setIsWindowOpen(bool isWindowOpen);
	Pacman* pacman();
	Ghost** ghosts();
	void restartAClock();
	sf::Time getElapsedTime();
	void changePacmanTextureRect();
	void startLogicThread();
	void startAnimationThread();
	

	static sf::Vector2f getNewCoords(sf::Vector2f currentCoords, sf::Vector2f speedVec);
	static bool isThereAnObstacleOnTheCoords(const TileMap& map, sf::Vector2f coords);
	static sf::Vector2f createSpeedVec(float pixelsPerSecond, int direction);
	static sf::Vector2f getForwardCenterPoint(sf::Vector2f entityCoords, sf::Vector2u spriteSize, int direction);
	static sf::Vector2f getABackPointInTheDirectionOfTheNextTurn(sf::Vector2f spriteCoords, sf::Vector2u spriteSize, int currentDirection, int nextDirection);
	static sf::Vector2f getAFrontPointInTheDirectionOfTheNextTurn(sf::Vector2f spriteCoords, sf::Vector2u spriteSize, int currentDirection, int nextDirection);
	
	static const int GHOST_COUNT{ 4 };

private:

	void logic();
	void animation();
	bool loadTextures();
	void loadSprites();
	bool loadMapForLvl(int levelNumb);
	void unloadMapFromLvl(int levelNumb);
	void pacmanSetupForLvl(int levelNumb);
	void ghostsSetupForLvl(int levelNumb);

	std::vector<TileMap*> m_levels; // Вектор с уровнями.
	std::string m_mapTileset; // Имя файла спрайтлиста уровня.
	std::string m_pacmanSpriteSet; // Имя файла спрайтлиста Пакмена.
	std::string m_ghostsSpriteSet; // Имя файла спрайтлиста Призраков.
	sf::Texture m_mapTilesetTexture;
	sf::Texture m_mapPacmanSetTexture;
	sf::Texture m_mapGhostsSetTexture;
	int m_levelNumber; // Номер текущего уровня.
	int m_animationCounter; // Счётчик кадров анимации.
	bool m_isWindowOpen; // Флаг, который говорит открыто ли окно программы.

	Pacman* m_pacman;
	Ghost** m_ghosts;

	sf::Thread m_logicThread;
	sf::Thread m_animationThread;
	sf::Mutex m_mutex;
	sf::Clock moveClock;
	sf::Clock animationClock;

};

