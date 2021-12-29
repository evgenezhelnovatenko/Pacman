#include "Game.h"

#include <functional>

const int Game::GHOST_COUNT;
sf::Vector2u Game::windowSize;

Game::Game(std::string mapTileset, std::string pacmanSpriteSet, std::string ghostsSpriteSet)
	: m_mapTileset{ mapTileset }
	, m_pacmanSpriteSet{ pacmanSpriteSet }
	, m_ghostsSpriteSet{ ghostsSpriteSet }
	, m_levelNumber{ 1 }
	, m_animationCounter{ 0 }
	, m_isWindowOpen{ true }
	, m_pacman{ new Pacman(sf::Vector2u(SPRITE_WIDTH, SPRITE_HEIGHT), MOVE_SPEED) }
	, m_ghosts{ new Ghost*[GHOST_COUNT] }
	, m_logicThread(&Game::logic, this)
	, m_animationThread(&Game::animation, this)
{
	for (int i = 0; i < GHOST_COUNT; i++) {
		m_ghosts[i] = new Ghost(sf::Vector2u(SPRITE_WIDTH, SPRITE_HEIGHT), MOVE_SPEED, i);
	}
}

Game::~Game()
{
	for (int i = 0; i < m_levels.size(); i++)
		delete m_levels.at(i);

	delete m_pacman;

	for (int i = 0; i < GHOST_COUNT; i++) {
		delete m_ghosts[i];
	}

	delete[] m_ghosts;
}

// ��������� ���� ����� ��������.
bool Game::setup()
{
	if (!loadTextures())
		return false;

	loadSprites();

	if (!levelPreparation(m_levelNumber))
		return false;

	return true;
}

// ���������� ������.
bool Game::levelPreparation(int levelNumb)
{
	if (!loadMapForLvl(levelNumb))
		return false;
	pacmanSetupForLvl(levelNumb);
	ghostsSetupForLvl(levelNumb);
}

void Game::addLevel(TileMap* newLevel)
{
	m_levels.push_back(newLevel);
}

TileMap* Game::currentLevel()
{
	return m_levels.at(m_levelNumber - 1);
}

// ��������� ������ �������� ������ �� ��������� (���� ����� �������).
void Game::goToTheNextLevel()
{
	if (m_levelNumber >= m_levels.size()) {
		m_levelNumber = m_levels.size() - 1;
		std::cout << "This is the last level. There are no more levels after him.";
	}
	m_levelNumber++;
}

// ��������� ������ �������� ������ �� ���������� (���� ����� �������).
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

Pacman* Game::pacman()
{
	return m_pacman;
}

Ghost** Game::ghosts()
{
	return m_ghosts;
}

// ������������� ����.
void Game::restartAClock()
{
	moveClock.restart();
}

// �������� �����, ��������� � ������� ������� ��� ����������� �����.
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

// ��������� ����� � ������� ����.
void Game::startLogicThread()
{
	m_logicThread.launch();
}

// ��������� ����� � ���������.
void Game::startAnimationThread()
{
	m_animationThread.launch();
}

// ��� ������ ���� (���������� � ��������� ������ m_logicThread).
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

			sf::Vector2f pacmanCenterPoint = getCenterPoint(m_pacman->coords(), m_pacman->spriteSize());
			int tileNumber = currentMap->getTileNumber(pacmanCenterPoint);
			int tileValue = currentMap->getTileValue(tileNumber);

			if (tileValue == TileMap::DOTTED_CELL) {
				currentMap->replacingADottedCellWithAnEmptyCell(tileNumber);
				//std::cout << currentMap->getTileValue(tileNumber);
				m_pacman->score++;
			}
			// ������� ��� �� ����� ������������.
			m_pacman->moveTo(newPacmanCoords);
		}

		moveClock.restart();
	}
	
	//std::cout << "� �����, ����������� ������� ��������, � �����, ������, ������ ��� ����.\n";

}

// ����� ������� �������� �� �������� (�������� � ��������� ������ m_animationThread).
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

// �������� ���� �������.
bool Game::loadTextures()
{
	if (!m_mapTilesetTexture.loadFromFile(m_mapTileset))
		return false;
	if (!m_mapPacmanSetTexture.loadFromFile(m_pacmanSpriteSet))
		return false;
	if (!m_mapGhostsSetTexture.loadFromFile(m_ghostsSpriteSet))
		return false;

	return true;
}

void Game::loadSprites()
{
	m_pacman->setTexture(&m_mapPacmanSetTexture);
	m_pacman->loadSprite();

	for (int i = 0; i < GHOST_COUNT; i++) {
		m_ghosts[i]->setTexture(&m_mapGhostsSetTexture);
		m_ghosts[i]->loadSprite();
	}
}

// �������� �����.
bool Game::loadMapForLvl(int levelNumb)
{
	TileMap* level = m_levels.at(levelNumb - 1);
	level->setTexture(&m_mapTilesetTexture);
	if (!level->load())
		return false;

	return true;
}

// �������� �����.
void Game::unloadMapFromLvl(int levelNumb)
{
	m_levels.at(levelNumb - 1)->unload();
}

// ��������� �������.
void Game::pacmanSetupForLvl(int levelNumb)
{
	TileMap* level = m_levels.at(levelNumb - 1);
	EntityParams* pacmanPrms = level->pacmanParams();
	m_pacman->setCoords(pacmanPrms->coords);
	m_pacman->setDirection(pacmanPrms->direction);
	m_pacman->updateSpeedVec(createSpeedVec(m_pacman->speed(), m_pacman->direction()));
}

// ��������� ���������.
void Game::ghostsSetupForLvl(int levelNumb)
{
	TileMap* level = m_levels.at(levelNumb - 1);
	EntityParams* ghostsPrms = level->ghostsParams();
	for (int i = 0; i < GHOST_COUNT; i++) {
		Ghost* ghost = m_ghosts[i];
		EntityParams* ghostPrms = &ghostsPrms[i];

		ghost->setCoords(ghostPrms->coords);
		ghost->setDirection(ghostPrms->direction);
		ghost->changeTextureRect(sf::IntRect(ghost->direction() * ghost->spriteSize().x, ghost->color() * ghost->spriteSize().y, ghost->spriteSize().x, ghost->spriteSize().y));
		ghost->updateSpeedVec(createSpeedVec(ghost->speed(), ghost->direction()));
	}
}


// static methods
sf::Vector2f Game::getNewCoords(sf::Vector2f currentCoords, sf::Vector2f speedVec)
{
	return currentCoords + speedVec;
}

// �������� �� ��, ���� �� �� ������� ����������� �����������.
bool Game::isThereAnObstacleOnTheCoords(const TileMap& map, sf::Vector2f coords)
{
	int tileNumber = map.getTileNumber(coords);
	int tileValue = map.getTileValue(tileNumber);

	if (tileValue != TileMap::DOTTED_CELL && tileValue != TileMap::EMPTY_CELL)
		return true;

	return false;
}

// �������� ������� �������� �� �������� � �����������.
sf::Vector2f Game::createSpeedVec(float pixelsPerSecond, int direction)
{
	float pixelsInOneHundredthOfASecond = pixelsPerSecond / 100; // ������� ���������� ��������, �� ������� ���� �������� ������ 0.01 �������.

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

sf::Vector2f Game::getCenterPoint(sf::Vector2f spriteCoords, sf::Vector2u spriteSize)
{

	return sf::Vector2f(spriteCoords.x + spriteSize.x / 2, spriteCoords.y + spriteSize.y / 2);
}

// �������� ����������� �������� ����� ������� �� ��� �����������, ������� � �����������.
sf::Vector2f Game::getForwardCenterPoint(sf::Vector2f spriteCoords, sf::Vector2u spriteSize, int direction)
{
	switch (direction)
	{
	case Direction::LEFT:
		spriteCoords.y += spriteSize.y / 2;
		break;
	case Direction::RIGHT:
		spriteCoords.x += spriteSize.x - 1;
		spriteCoords.y += spriteSize.y / 2;
		break;
	case Direction::UP:
		spriteCoords.x += spriteSize.x / 2;
		break;
	case Direction::DOWN:
		spriteCoords.x += spriteSize.x / 2;
		spriteCoords.y += spriteSize.y - 1;
		break;
	default:
		break;
	}

	return spriteCoords;
}

// �������� ������ ����� ������� �� ��� ���������� �����������, ����������� � �������.
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

// �������� �������� ����� ������� �� ��� ���������� �����������, ����������� � �������.
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
		else if (nextDirection == Direction::DOWN) {
			requiredPoint.x += spriteSize.x - 1;
			requiredPoint.y += spriteSize.y - 1;
		}
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

sf::Vector2f Game::getGlobalPosition(sf::Vector2f currCoords, TileMap* level)
{
	int levelWidth = level->widthInPixels();
	int levelHeight = level->heightInPixels();

	int valueToAddToWidth = (windowSize.x - levelWidth) / 2;
	int valueToAddToHeight = (windowSize.y - levelHeight) / 2;

	currCoords.x += valueToAddToWidth;
	currCoords.y += valueToAddToHeight;

	return currCoords;
}

sf::Vector2f Game::centeringTheMap(TileMap* level)
{
	sf::VertexArray* mapVertices = level->vertices();
	for (unsigned int i = 0; i < level->width(); ++i) {
		for (unsigned int j = 0; j < level->height(); ++j) {
			sf::Vertex* quad = &(*mapVertices)[(i + j * level->width()) * 4];

			quad[0].position = Game::getGlobalPosition(quad[0].position, level);
			quad[1].position = Game::getGlobalPosition(quad[1].position, level);
			quad[2].position = Game::getGlobalPosition(quad[2].position, level);
			quad[3].position = Game::getGlobalPosition(quad[3].position, level);
		}
	}
	return sf::Vector2f();
}

std::string Game::convertIntToString(int input, int numberOfDigitsInALine)
{
	std::string output = "";
	int intPartOfNumb = input;
	int lastDigitOfANumber = input;

	for (int j = 0; j < numberOfDigitsInALine; j++) {
		if (intPartOfNumb != 0) {
			lastDigitOfANumber = intPartOfNumb % 10;
			intPartOfNumb /= 10;
			output.insert(0, std::to_string(lastDigitOfANumber));
		}
		else
			output.insert(0, "0");
	}


	return output;
}
