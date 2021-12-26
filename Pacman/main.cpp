
#include <iostream>
#include <vector>
#include <functional>

#include "SFML/Graphics.hpp"

#include "Game.h"
#include "Enums.h"

#define PACMAN_SPEED 100.f


int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(512, 450), "Pac-man");
	window.setFramerateLimit(60);

	Game game("images/map_tileset.png", "images/pacman_spriteSet.png", "images/ghosts_spriteSet.png");

	Game::windowSize = window.getSize();

	// ќбъ€вление уровней, которые состо€т из массивов индексов спрайтов.
	int level1[] =
	{
		7,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	5,
		12,	22,	22, 22, 22, 22, 22, 22,	22,	22,	22,	22,	22,	22,	22,	13,
		12,	22,	18,	22, 11, 19, 22, 21, 9,	22, 11, 20, 20, 19,	22, 13,
		12,	22,	17,	22, 17, 22, 22, 22, 17, 22, 17, 22, 22, 22,	22, 13,
		12,	22,	17,	22, 17, 22, 22, 22, 17, 22, 17, 22, 11, 19,	22, 13,
		12,	22,	16,	22, 10, 20, 20, 20, 8,	22, 16, 22, 17, 22,	22, 13,
		12,	22,	22,	22,	22, 22, 22, 22,	22,	22, 22, 22, 10,	9,	22,	13,
		12,	22,	11,	20, 20, 20, 20, 20, 20, 20, 9,	22,	22, 17,	22,	13,
		12,	22,	17,	22, 22, 22, 22, 22, 22, 22, 10, 9,	22, 17, 22,	13,
		12,	22,	17,	22, 22, 22, 22, 22, 22, 22, 22, 17, 22, 17,	22,	13,
		12,	22,	10,	20, 20, 20, 20, 20, 20, 20, 20,	8,	22, 16,	22,	13,
		12,	22,	22, 22, 22,	22, 22, 22,	22,	22,	22,	22,	22,	22,	22,	13,
		6,	15,	15,	15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,	15,	4
	};
	EntityParams ghostsPrmsForLvl1[] = {
		EntityParams(sf::Vector2f(150, 90), Direction::RIGHT),
		EntityParams(sf::Vector2f(200, 90), Direction::LEFT),
		EntityParams(sf::Vector2f(150, 120), Direction::UP),
		EntityParams(sf::Vector2f(200, 120), Direction::DOWN)
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
		EntityParams(sf::Vector2f(230, 60), Direction::LEFT),
		EntityParams(sf::Vector2f(180, 90), Direction::UP),
		EntityParams(sf::Vector2f(230, 90), Direction::DOWN)
	};

	game.addLevel(new TileMap(sf::Vector2u(30, 30), level1, 16, 13, new EntityParams(sf::Vector2f(30, 30), Direction::RIGHT), ghostsPrmsForLvl1));
	game.addLevel(new TileMap(sf::Vector2u(30, 30), level2, 16, 8, new EntityParams(sf::Vector2f(200, 30), Direction::RIGHT), ghostsPrmsForLvl2));

	if (!game.setup())
		return -1;

	TileMap* currentMap = game.currentLevel(); // ”казатель на текущий уровень.

	sf::VertexArray* mapVertices = currentMap->vertices();
	for (unsigned int i = 0; i < currentMap->width(); ++i) {
		for (unsigned int j = 0; j < currentMap->height(); ++j) {
			sf::Vertex* quad = &(*mapVertices)[(i + j * currentMap->width()) * 4];

			quad[0].position = Game::getGlobalPosition(quad[0].position, game.currentLevel());
			quad[1].position = Game::getGlobalPosition(quad[1].position, game.currentLevel());
			quad[2].position = Game::getGlobalPosition(quad[2].position, game.currentLevel());
			quad[3].position = Game::getGlobalPosition(quad[3].position, game.currentLevel());
		}
	}

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
					
					if (pacman->direction() != Direction::LEFT)
						pacman->setNextDirection(Direction::LEFT);

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);
	
					break;
				case sf::Keyboard::Right:
					if (pacman->direction() != Direction::RIGHT)
						pacman->setNextDirection(Direction::RIGHT);

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);

					break;
				case sf::Keyboard::Up:
					if (pacman->direction() != Direction::UP)
						pacman->setNextDirection(Direction::UP);

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);

					break;
				case sf::Keyboard::Down:
					if (pacman->direction() != Direction::DOWN)
						pacman->setNextDirection(Direction::DOWN);

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

		
		Ghost** ghosts = game.ghosts();
		
		// ќчищаем окно.
		window.clear();
		// –исуем уровень.
		
		window.draw(*currentMap);
		// –исуем пакмена.
		
		pacman->setPosition(Game::getGlobalPosition(pacman->coords(), game.currentLevel()));
		window.draw(*pacman);
		// –исуем призраков.
		for (int i = 0; i < Game::GHOST_COUNT; i++) {
			Ghost* ghost = ghosts[i];
			ghost->setPosition(Game::getGlobalPosition(ghost->coords(), game.currentLevel()));
			window.draw(*(ghosts[i]));
		}

		window.display();
	}

	game.setIsWindowOpen(false);

	return 0;
}