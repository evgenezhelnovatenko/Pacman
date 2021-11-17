
#include <iostream>
#include <vector>

#include "SFML/Graphics.hpp"
#include "TileMap.h"
#include "Pacman.h"
#include "Direction.h"


sf::Vector2f getNewCoords(sf::Vector2f currentCoords, sf::Vector2f speedVec) {
	return currentCoords + speedVec;
}

bool isThereAnObstacleOnTheCoords(const TileMap& map, sf::Vector2f coords) { // Проверка на то, есть ли по заданым кооридинатам, на заданой карте препятствие.
	int tileNumber = map.getTileNumber(coords);
	int tileValue = map.getTileValue(tileNumber);

	if (tileValue != 22)
		return true;

	return false;
}

void updateCurrentMap(const std::vector<TileMap*> &levels, TileMap *&currentMap, int levelNumber) { // Обновление текущей карты.

	currentMap = levels.at(levelNumber - 1);
}

sf::Vector2f createSpeedVec(float speed, int direction) {
	
	sf::Vector2f speedVec;

	switch (direction)
	{
	case Direction::RIGHT:
		speedVec = sf::Vector2f(1, 0) * speed;
		break;
	case Direction::LEFT:
		speedVec = sf::Vector2f(-1, 0) * speed;
		break;
	case Direction::UP:
		speedVec = sf::Vector2f(0, -1) * speed;
		break;
	case Direction::DOWN:
		speedVec = sf::Vector2f(0, 1) * speed;
		break;
	default:
		break;
	}

	return speedVec;
}

sf::Vector2f getForwardCenterPoint(sf::Vector2f entityCoords, sf::Vector2u spriteSize, int direction) { // Получение координат переднего центрального узла спрайта.

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

sf::Vector2f getABackPointInTheDirectionOfTheNextTurn(sf::Vector2f spriteCoords, sf::Vector2u spriteSize, int currentDirection, int nextDirection) {

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

sf::Vector2f getAFrontPointInTheDirectionOfTheNextTurn(sf::Vector2f spriteCoords, sf::Vector2u spriteSize, int currentDirection, int nextDirection) {

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

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 256), "Pac-man");
	window.setFramerateLimit(20);


	// define the level with an array of tile indices
	int level_1[] =
	{
		7,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	5,
		12,	22,	22, 22, 22, 22, 22, 22,	22,	22,	22,	22,	22,	22,	22,	13,
		12,	22,	11,	20,	20,	20,	20, 20, 20, 20, 20, 20, 20, 9,	22, 13,
		12,	22,	17,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 17,	22, 13,
		12,	22,	17,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 17,	22, 13,
		12,	22,	10, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 8,	22, 13,
		12,	22,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 13,
		6,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	4,
	};
	int level_2[] =
	{
		7,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	5,
		12,	22,	22, 22, 22, 22, 22, 22,	22,	22,	22,	22,	22,	22,	22,	13,
		12,	22,	11,	20,	20,	20,	20, 20, 20, 20, 20, 20, 20, 9,	22, 13,
		12,	22,	17,	22, 22, 22, 22, 13, 22, 22, 22, 22, 22, 17,	22, 13,
		12,	22,	17,	22, 22, 22, 22, 13, 22, 22, 22, 22, 22, 17,	22, 13,
		12,	22,	10, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 8,	22, 13,
		12,	22,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 13,
		6,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	15,	4,
	};

	std::vector<TileMap*> levels; // Вектор с массивами уровней.
	int levelNumber = 1; // Номер текущего уровня.

	// Наполняем вектор уровней создаными картами.
	levels.push_back(new TileMap(sf::Vector2u(30, 30), level_1, 16, 8));
	levels.push_back(new TileMap(sf::Vector2u(30, 30), level_2, 16, 8));

	// Загружаем для каждого уровня спрайтлист. Если хоть один спрайтлист не загрузиться - программа закончит роботу.
	for (int i = 0; i < levels.size(); i++) {
		if (!levels.at(i)->load("images/map_tileset.png"))
			return -1;
	}
	
	TileMap* currentMap = levels.at(levelNumber - 1); // Указатель на текущий уровень.

	// Созадние объекта Пакмена и загрузка спрайтлиста для него. Если спрайтлист не загрузиться - программа закончит роботу.
	Pacman pacman(sf::Vector2f(30, 30), sf::Vector2u(30, 30), 3, Direction::RIGHT, createSpeedVec(3, Direction::RIGHT));
	if (!pacman.loadSprite("images/PacmanSprite_30x30.png"))
	{
		return -1;
	}

	int animationCounter = 0; // Счётчик кадров анимации.

	// run the main loop
	while (window.isOpen())
	{
		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// window closed
			case sf::Event::Closed:
				window.close();
				break;

				// key pressed
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Left:
					
					if (pacman.direction() == Direction::UP || pacman.direction() == Direction::DOWN)
						pacman.setNextDirection(Direction::LEFT);
					else {
						pacman.setDirection(Direction::LEFT);
						pacman.updateSpeedVec();
					}

					if (pacman.isStandStill()) 
						pacman.setIsStandStill(false);
	
					break;
				case sf::Keyboard::Right:
					if (pacman.direction() == Direction::UP || pacman.direction() == Direction::DOWN)
						pacman.setNextDirection(Direction::RIGHT);
					else {
						pacman.setDirection(Direction::RIGHT);
						pacman.updateSpeedVec();
					}

					if (pacman.isStandStill())
						pacman.setIsStandStill(false);
					break;
				case sf::Keyboard::Up:
					if (pacman.direction() == Direction::LEFT || pacman.direction() == Direction::RIGHT)
						pacman.setNextDirection(Direction::UP);
					else {
						pacman.setDirection(Direction::UP);
						pacman.updateSpeedVec();
					}

					if (pacman.isStandStill())
						pacman.setIsStandStill(false);
					break;
				case sf::Keyboard::Down:
					if (pacman.direction() == Direction::LEFT || pacman.direction() == Direction::RIGHT)
						pacman.setNextDirection(Direction::DOWN);
					else {
						pacman.setDirection(Direction::DOWN);
						pacman.updateSpeedVec();
					}

					if (pacman.isStandStill())
						pacman.setIsStandStill(false);
					break;
				default:
					break;
				}
				
				break;

				// we don't process other types of events
			default:
				break;
			}

		}

		if (!pacman.isStandStill()) {

			sf::Vector2f newPacmanCoords;
			sf::Vector2f coordsToCheck;

			if (pacman.nextDirection() != -1) {

				bool canGo = true;
				newPacmanCoords = getNewCoords(pacman.coords(), createSpeedVec(pacman.speed(), pacman.nextDirection()));
				coordsToCheck = getABackPointInTheDirectionOfTheNextTurn(newPacmanCoords, pacman.spriteSize(), pacman.direction(), pacman.nextDirection());

				if (isThereAnObstacleOnTheCoords(*currentMap, coordsToCheck))
					canGo = false;

				if (canGo) {
					coordsToCheck = getAFrontPointInTheDirectionOfTheNextTurn(newPacmanCoords, pacman.spriteSize(), pacman.direction(), pacman.nextDirection());
					if (isThereAnObstacleOnTheCoords(*currentMap, coordsToCheck))
						canGo = false;
				}


				if (canGo) {
					pacman.updateTheCurrentDirectionToTheNext();
					pacman.updateSpeedVec();
					pacman.moveTo(newPacmanCoords);
				}
			}

			// Высчитывание координат для проверки на препятствие спереди.
			newPacmanCoords = getNewCoords(pacman.coords(), pacman.speedVec());

			coordsToCheck = getForwardCenterPoint(newPacmanCoords,
				pacman.spriteSize(),
				pacman.direction());

			// Если препятствие есть, то подвигаем Пакмена на координаты плитки, на которой он стоял.
			if (isThereAnObstacleOnTheCoords(*currentMap, coordsToCheck)) {
				pacman.setIsStandStill(true);
				int tileNumber = currentMap->getTileNumber(getForwardCenterPoint(pacman.coords(),
					pacman.spriteSize(),
					pacman.direction()));
				newPacmanCoords = currentMap->getTileCoords(tileNumber);
			}
			// Иначе двигаем его по новым кооридинатам.
			pacman.moveTo(newPacmanCoords);
			

			// Изменение счётчика кадров анимации.
			if (++animationCounter > 3)
				animationCounter = 0;

			// Меням координаты спрайта, который нужно использовать, на текстуре.
			pacman.changeTextureRect(sf::IntRect(pacman.spriteSize().x * animationCounter,
				pacman.spriteSize().y * pacman.direction(), 
				pacman.spriteSize().x,
				pacman.spriteSize().y));
		}
		
		// draw the map
		window.clear();
		window.draw(*currentMap);
		window.draw(pacman);
		window.display();
	}

	return 0;
}