
#include <iostream>
#include <vector>
#include <functional>

#include "SFML/Graphics.hpp"

#include "Game.h"
#include "Direction.h"

#define PACMAN_SPEED 100.f


int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 450), "Pac-man");
	window.setFramerateLimit(60);

	Game game("images/map_tileset.png", "images/pacman_spriteSet.png", "images/ghosts_spriteSet.png");

	// ќбъ€вление уровней, которые состо€т из массивов индексов спрайтов.
	int level1[] =
	{
		7,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	5,
		12,	22,	22, 22, 22, 22, 22, 22,	22,	22,	22,	22,	22,	22,	22,	13,
		12,	22,	3,	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 1,	22, 13,
		12,	22,	13,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 12,	22, 13,
		12,	22,	2,	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 0,	22, 13,
		12,	22,	22,	22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,	22, 13,
		6,	1,	22,	3,	15, 15, 15, 1,	3,	15, 15, 15, 1,	22,	3,	4,
		22,	12,	22,	13, 22, 22, 22, 12, 13, 22, 22, 22, 12,	22,	13, 22,
		22,	12,	22,	13, 14, 14, 14, 12, 13, 14, 14, 14, 12, 22,	13, 22,
		22,	12,	22,	2,	14, 14, 14, 0,	2,	14, 14, 14, 0,	22, 13, 22,
		22,	12,	22, 22, 22, 22, 22, 22,	22,	22,	22,	22,	22,	22,	13,	22,
		22,	12,	22, 22, 22,	3,	15,	15,	15,	15,	1,	22, 22, 22,	13,	22,
		22,	6,	15,	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,	4,	22,
	};
	EntityParams ghostsPrmsForLvl1[] = {
		EntityParams(sf::Vector2f(180, 60), Direction::RIGHT),
		EntityParams(sf::Vector2f(200, 60), Direction::LEFT),
		EntityParams(sf::Vector2f(180, 90), Direction::UP),
		EntityParams(sf::Vector2f(200, 90), Direction::DOWN)
	};

	int level2[] =
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
	EntityParams ghostsPrmsForLvl2[] = {
		EntityParams(sf::Vector2f(180, 60), Direction::RIGHT),
		EntityParams(sf::Vector2f(200, 60), Direction::LEFT),
		EntityParams(sf::Vector2f(180, 90), Direction::UP),
		EntityParams(sf::Vector2f(200, 90), Direction::DOWN)
	};

	game.addLevel(new TileMap(sf::Vector2u(30, 30), level1, 16, 13, new EntityParams(sf::Vector2f(30, 30), Direction::RIGHT), ghostsPrmsForLvl1));
	game.addLevel(new TileMap(sf::Vector2u(30, 30), level2, 16, 8, new EntityParams(sf::Vector2f(200, 30), Direction::RIGHT), ghostsPrmsForLvl2));

	if (!game.setup())
		return -1;

	Pacman* pacman = game.pacman();

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

		TileMap* currentMap = game.currentLevel(); // ”казатель на текущий уровень.
		Ghost** ghosts = game.ghosts();
		
		// ќчищаем окно.
		window.clear();
		// –исуем уровень.
		window.draw(*currentMap);
		// –исуем пакмена.
		window.draw(*pacman);
		std::cout << "x: " << pacman->coords().x << "; y: " << pacman->coords().y << std::endl;
		// –исуем призраков.
		for (int i = 0; i < Game::GHOST_COUNT; i++) {
			window.draw(*(ghosts[i]));
		}

		window.display();
	}

	game.setIsWindowOpen(false);

	return 0;
}