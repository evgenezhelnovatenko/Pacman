#include "Game.h"

#include <functional>


Game::Game()
	: m_levelNumber{ 1 }
	, m_animationCounter{ 0 }
	, m_isWindowOpen{ true }
	, m_pacman{ nullptr }
	, m_logicThread(&Game::logic, this)
	, m_animationThread(&Game::animation, this)
{

}

Game::~Game()
{
	for (int i = 0; i < m_levels.size(); i++)
		delete m_levels.at(i);
}

void Game::addLevel(TileMap* newLevel)
{
	m_levels.push_back(newLevel);
}

bool Game::loadTilesetForEveryLevel(std::string tileset)
{
	for (int i = 0; i < m_levels.size(); i++) {
		if (!m_levels.at(i)->load(tileset))
			return false;
	}
}

TileMap* Game::currentLevel()
{
	return m_levels.at(m_levelNumber - 1);
}


void Game::goToTheNextLevel()
{
	if (m_levelNumber >= m_levels.size()) {
		m_levelNumber = m_levels.size() - 1;
		std::cout << "This is the last level. There are no more levels after him.";
	}
	m_levelNumber++;
}

void Game::goToThePreviousLevel()
{
	if (m_levelNumber - 1 <= 0) {
		m_levelNumber = 1;
		std::cout << "This is the first level. There are no more levels in front of him.";
	}
	else
		m_levelNumber--;
}

void Game::setIsWindowOpen(bool isWindowOpen)
{
	if (m_isWindowOpen == isWindowOpen)
		return;

	m_isWindowOpen = isWindowOpen;
}

void Game::setPacman(Pacman* pacman)
{
	if (m_pacman == pacman)
		return;

	m_pacman = pacman;
}

Pacman*& Game::pacman()
{
	return m_pacman;
}

void Game::restartAClock()
{
	moveClock.restart();
}

sf::Time Game::getElapsedTime()
{
	return moveClock.getElapsedTime();
}

void Game::changePacmanTextureRect()
{
	m_mutex.lock();
	m_pacman->changeTextureRect(sf::IntRect(m_pacman->spriteSize().x * m_animationCounter,
		m_pacman->spriteSize().y * m_pacman->direction(),
		m_pacman->spriteSize().x,
		m_pacman->spriteSize().y));
	m_mutex.unlock();
}

void Game::startLogicThread()
{
	m_logicThread.launch();
}

void Game::startAnimationThread()
{
	m_animationThread.launch();
}

void Game::logic() {

	startAnimationThread();

	while (m_isWindowOpen)
	{

		sf::Time time = moveClock.getElapsedTime();
		if (time.asSeconds() < 0.01)
			continue;


		if (!m_pacman->isStandStill()) {

			TileMap* currentMap = currentLevel();

			sf::Vector2f newPacmanCoords;
			sf::Vector2f coordsToCheck;
			sf::Vector2f nextSpeedVec;

			if (m_pacman->nextDirection() != -1) { // ���� � ������� ������ ����������, ����� �������� �����������, �� ��������� ����� �� ������ ��������� � ������� ����������� �����������.

				bool canGo = true;
				nextSpeedVec = createSpeedVec(m_pacman->speed(), m_pacman->nextDirection());
				newPacmanCoords = getNewCoords(m_pacman->coords(), nextSpeedVec); // ������� ���������� �������.
				coordsToCheck = getABackPointInTheDirectionOfTheNextTurn(newPacmanCoords, m_pacman->spriteSize(), m_pacman->direction(), m_pacman->nextDirection()); // ���������� ������ ����� ������������ ������� ��������� �������.
																																							// ��� ����� ��� �������� ����, ������ �� ������ ����� ������� �� ��������.

				// ���� ����� ��� �������� ���������� �� �����������, �� canGo ����������� false.
				if (isThereAnObstacleOnTheCoords(*currentMap, coordsToCheck))
					canGo = false;

				// ���� ������ �� ��������� �� ����������� ������ ������ - ��������� ��������.
				if (canGo) {
					coordsToCheck = getAFrontPointInTheDirectionOfTheNextTurn(newPacmanCoords, m_pacman->spriteSize(), m_pacman->direction(), m_pacman->nextDirection()); // ���������� �������� ����� ������������ ������� ��������� �������.
					if (isThereAnObstacleOnTheCoords(*currentMap, coordsToCheck)) // ���� �������� ����� ���������� �� �����������, �� canGo ����������� false.
						canGo = false;
				}

				// ���� canGo == true, ������ ������ ����� ���������.
				if (canGo) {
					m_pacman->updateTheCurrentDirectionToTheNext(); // �������� ������� ����������� �� ��������� � ������� ��������� �����������.
					changePacmanTextureRect(); // ������ ���� ���������
					m_pacman->updateSpeedVec(nextSpeedVec); // ��������� ������ ��������.
					//m_pacman->moveTo(newPacmanCoords); // ����� ������� ����� ���������� (������� ���).
				}
			}

			// ������������ ��������� ��� �������� �� ����������� �������.
			newPacmanCoords = getNewCoords(m_pacman->coords(), m_pacman->speedVec());

			coordsToCheck = getForwardCenterPoint(newPacmanCoords,
				m_pacman->spriteSize(),
				m_pacman->direction());

			// ���� ����������� ����, �� ����� ������� ���������� ������, �� ������� �� �����.
			if (isThereAnObstacleOnTheCoords(*currentMap, coordsToCheck)) {
				m_pacman->setIsStandStill(true);
				int tileNumber = currentMap->getTileNumber(getForwardCenterPoint(m_pacman->coords(),
					m_pacman->spriteSize(),
					m_pacman->direction()));
				newPacmanCoords = currentMap->getTileCoords(tileNumber);
			}
			// ������� ��� �� ����� ������������.
			m_pacman->moveTo(newPacmanCoords);
		}

		moveClock.restart();
	}
	
	//std::cout << "� �����, ����������� ������� ��������, � �����, ������, ������ ��� ����.\n";

}

void Game::animation()
{
	while (m_isWindowOpen)
	{
		sf::Time time = animationClock.getElapsedTime();
		if (time.asSeconds() < (1.0 / ANIMATION_SPEED))
			continue;

		if (!m_pacman->isStandStill()) {
			// ��������� �������� ������ ��������.
			if (++m_animationCounter > 3)
				m_animationCounter = 0;

			// ����� ���������� �������, ������� ����� ������������, �� ��������.
			changePacmanTextureRect();
		}
		animationClock.restart();
	}

}


// static methods
sf::Vector2f Game::getNewCoords(sf::Vector2f currentCoords, sf::Vector2f speedVec)
{
	return currentCoords + speedVec;
}

bool Game::isThereAnObstacleOnTheCoords(const TileMap& map, sf::Vector2f coords)
{
	int tileNumber = map.getTileNumber(coords);
	int tileValue = map.getTileValue(tileNumber);

	if (tileValue != 22)
		return true;

	return false;
}

sf::Vector2f Game::createSpeedVec(float pixelsPerSecond, int direction)
{
	float pixelsInOneHundredthOfASecond = pixelsPerSecond / 100; // ������� ���������� ��������, �� ������� ���� �������� ������ 0.1 �������.

	sf::Vector2f speedVec;

	switch (direction)
	{
	case Direction::RIGHT:
		speedVec = sf::Vector2f(1, 0) * pixelsInOneHundredthOfASecond;
		break;
	case Direction::LEFT:
		speedVec = sf::Vector2f(-1, 0) * pixelsInOneHundredthOfASecond;
		break;
	case Direction::UP:
		speedVec = sf::Vector2f(0, -1) * pixelsInOneHundredthOfASecond;
		break;
	case Direction::DOWN:
		speedVec = sf::Vector2f(0, 1) * pixelsInOneHundredthOfASecond;
		break;
	default:
		break;
	}

	return speedVec;
}

sf::Vector2f Game::getForwardCenterPoint(sf::Vector2f entityCoords, sf::Vector2u spriteSize, int direction)
{
	switch (direction)
	{
	case Direction::LEFT:
		entityCoords.y += spriteSize.y / 2;
		break;
	case Direction::RIGHT:
		entityCoords.x += spriteSize.x - 1;
		entityCoords.y += spriteSize.y / 2;
		break;
	case Direction::UP:
		entityCoords.x += spriteSize.x / 2;
		break;
	case Direction::DOWN:
		entityCoords.x += spriteSize.x / 2;
		entityCoords.y += spriteSize.y - 1;
		break;
	default:
		break;
	}

	return entityCoords;
}

sf::Vector2f Game::getABackPointInTheDirectionOfTheNextTurn(sf::Vector2f spriteCoords, sf::Vector2u spriteSize, int currentDirection, int nextDirection)
{
	sf::Vector2f requiredPoint = spriteCoords;

	switch (currentDirection)
	{
	case Direction::LEFT:

		if (nextDirection == Direction::UP)
			requiredPoint.x += spriteSize.x - 1;
		else if (nextDirection == Direction::DOWN) {
			requiredPoint.x += spriteSize.x - 1;
			requiredPoint.y += spriteSize.y - 1;
		}
		break;
	case Direction::RIGHT:

		if (nextDirection == Direction::DOWN)
			requiredPoint.y += spriteSize.y - 1;
		break;
	case Direction::UP:

		if (nextDirection == Direction::RIGHT) {
			requiredPoint.x += spriteSize.x - 1;
			requiredPoint.y += spriteSize.y - 1;
		}
		else if (nextDirection == Direction::LEFT)
			requiredPoint.y += spriteSize.y - 1;
		break;
	case Direction::DOWN:

		if (nextDirection == Direction::RIGHT)
			requiredPoint.x += spriteSize.x - 1;
		break;
	default:
		break;
	}

	return requiredPoint;
}

sf::Vector2f Game::getAFrontPointInTheDirectionOfTheNextTurn(sf::Vector2f spriteCoords, sf::Vector2u spriteSize, int currentDirection, int nextDirection)
{
	sf::Vector2f requiredPoint = spriteCoords;

	switch (currentDirection)
	{
	case Direction::LEFT:

		if (nextDirection == Direction::DOWN)
			requiredPoint.y += spriteSize.y - 1;
		break;
	case Direction::RIGHT:

		if (nextDirection == Direction::UP)
			requiredPoint.x += spriteSize.x - 1;
		else if (nextDirection == Direction::DOWN)
			requiredPoint.x += spriteSize.x - 1;
		requiredPoint.y += spriteSize.y - 1;
		break;
	case Direction::UP:

		if (nextDirection == Direction::RIGHT)
			requiredPoint.x += spriteSize.x - 1;
		break;
	case Direction::DOWN:

		if (nextDirection == Direction::LEFT)
			requiredPoint.y += spriteSize.y - 1;
		else if (nextDirection == Direction::RIGHT) {
			requiredPoint.x += spriteSize.x - 1;
			requiredPoint.y += spriteSize.y - 1;
		}
		break;
	default:
		break;
	}

	return requiredPoint;
}

