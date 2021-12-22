
#include <iostream>
#include <vector>
#include <functional>

#include "SFML/Graphics.hpp"
#include "Game.h"
#include "TileMap.h"
#include "Pacman.h"
#include "Direction.h"

#define PACMAN_SPEED 100.f

int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 400), "Pac-man");
	window.setFramerateLimit(60);

	Game game;

	// Объявление уровней, которые состоят из массивов индексов спрайтов.
	int level_1[] =
	{
		7,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	5,
		12,	22,	22, 22, 22, 22, 22, 22,	22,	22,	22,	22,	22,	22,	22,	13,
		12,	22,	3,	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1,	22, 13,
		12,	22,	13,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 12,	22, 13,
		12,	22,	2,	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0,	22, 13,
		12,	22,	22,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,	22, 13,
		12,	22,	3,	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1,	22, 13,
		12,	22,	13,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 12,	22, 13,
		12,	22,	2,	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0,	22, 13,
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

	// Созадние объекта Пакмена и загрузка спрайтлиста для него. Если спрайтлист не загрузиться - программа закончит роботу.
	Pacman* pacman = new Pacman(sf::Vector2f(30, 30), sf::Vector2u(30, 30), PACMAN_SPEED, Direction::RIGHT, Game::createSpeedVec(PACMAN_SPEED, Direction::RIGHT));
	if (!pacman->loadSprite("images/PacmanSprite_30x30.png"))
	{
		return -1;
	}

	game.addLevel(new TileMap(sf::Vector2u(30, 30), level_1, 16, 11));
	game.addLevel(new TileMap(sf::Vector2u(30, 30), level_2, 16, 8));

	// Загружаем для каждого уровня спрайтлист. Если хоть один спрайтлист не загрузиться - программа закончит роботу.
	if (!game.loadTilesetForEveryLevel("images/map_tileset.png"))
		return -1;

	game.setPacman(pacman);

	TileMap* currentMap = game.currentLevel(); // Указатель на текущий уровень.

	game.startLogicThread();

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
					
					if (pacman->direction() == Direction::UP || pacman->direction() == Direction::DOWN)
						pacman->setNextDirection(Direction::LEFT);
					else if (pacman->direction() == Direction::RIGHT) {
						pacman->setDirection(Direction::LEFT);
						pacman->updateSpeedVec(Game::createSpeedVec(pacman->speed(), pacman->direction()));
						if (pacman->nextDirection() != -1)
							pacman->setNextDirection(-1);
					}

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);
	
					break;
				case sf::Keyboard::Right:
					if (pacman->direction() == Direction::UP || pacman->direction() == Direction::DOWN)
						pacman->setNextDirection(Direction::RIGHT);
					else if (pacman->direction() == Direction::LEFT) {
						pacman->setDirection(Direction::RIGHT);
						pacman->updateSpeedVec(Game::createSpeedVec(pacman->speed(), pacman->direction()));
						if (pacman->nextDirection() != -1)
							pacman->setNextDirection(-1);
					}

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);
					break;
				case sf::Keyboard::Up:
					if (pacman->direction() == Direction::LEFT || pacman->direction() == Direction::RIGHT)
						pacman->setNextDirection(Direction::UP);
					else if (pacman->direction() == Direction::DOWN) {
						pacman->setDirection(Direction::UP);
						pacman->updateSpeedVec(Game::createSpeedVec(pacman->speed(), pacman->direction()));
						if (pacman->nextDirection() != -1)
							pacman->setNextDirection(-1);
					}

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);
					break;
				case sf::Keyboard::Down:
					if (pacman->direction() == Direction::LEFT || pacman->direction() == Direction::RIGHT)
						pacman->setNextDirection(Direction::DOWN);
					else if (pacman->direction() == Direction::UP) {
						pacman->setDirection(Direction::DOWN);
						pacman->updateSpeedVec(Game::createSpeedVec(pacman->speed(), pacman->direction()));
						if (pacman->nextDirection() != -1)
							pacman->setNextDirection(-1);
					}

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);
					break;
				default:
					break;
				}
				
				break;

			default:
				break;
			}

		}


		// Игровая логика.
		

		
		// Очищаем окно.
		window.clear();
		// Рисуем уровень.
		window.draw(*currentMap);
		// Рисуем пакмена.
		window.draw(*pacman);

		window.display();
	}

	game.setIsWindowOpen(false);

	
	delete pacman;

	return 0;
}