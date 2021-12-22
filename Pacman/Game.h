#pragma once
#include <vector>
#include <iostream>

#include "TileMap.h"
#include "Pacman.h"

#define ANIMATION_SPEED 6

class Game
{
public:
	Game();
	~Game();

	void addLevel(TileMap* newLevel);
	bool loadTilesetForEveryLevel(std::string tileset);
	TileMap* currentLevel();
	void goToTheNextLevel();
	void goToThePreviousLevel();
	void setIsWindowOpen(bool isWindowOpen);
	void setPacman(Pacman* pacman);
	Pacman*& pacman();
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
	

	

private:

	void logic();
	void animation();

	std::vector<TileMap*> m_levels; // Вектор с уровнями.
	int m_levelNumber; // Номер текущего уровня.
	int m_animationCounter; // Счётчик кадров анимации.
	bool m_isWindowOpen;

	Pacman* m_pacman;
	sf::Thread m_logicThread;
	sf::Thread m_animationThread;
	sf::Mutex m_mutex;
	sf::Clock moveClock;
	sf::Clock animationClock;
};

