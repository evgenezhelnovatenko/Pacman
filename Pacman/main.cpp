
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

	sf::Text scoreText;
	sf::Font font;

	if (!font.loadFromFile("fonts/sunday.ttf")) {
		return -1;
	}

	scoreText.setFont(font);
	scoreText.setFillColor(sf::Color::Red);
	scoreText.setLetterSpacing(2.5);
	scoreText.setStyle(sf::Text::Italic);
	scoreText.setString("000000");
	scoreText.setPosition(window.getSize().x - 160, 0);

	Game game("images/map_tileset.png", "images/pacman_spriteSet.png", "images/ghosts_spriteSet.png");

	Game::windowSize = window.getSize();

	// ќбъ€вление уровней, которые состо€т из массивов индексов спрайтов.
	int level1[] =
	{
		7,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	14,	5,
		12,	46,	46, 46, 46, 46, 46, 46,	46,	46,	46,	46,	46,	46,	46,	13,
		12,	46,	18,	46, 11, 19, 47, 21, 9,	46, 11, 20, 20, 19,	46, 13,
		12,	46,	17,	46, 17, 47, 47, 47, 17, 46, 17, 46, 46, 46,	46, 13,
		12,	46,	17,	46, 17, 47, 47, 47, 17, 46, 17, 46, 11, 19,	46, 13,
		12,	46,	16,	46, 10, 20, 20, 20, 8,	46, 16, 46, 17, 46,	46, 13,
		12,	46,	46,	46,	46, 46, 46, 46,	46,	46, 46, 46, 10,	9,	46,	13,
		30,	19,	46,	11, 20, 20, 20, 37, 15, 15,	1,	46,	46, 17,	46,	13,
		12,	46,	46,	17, 46, 46, 46, 13, 7,	14,	36, 1,	46, 17, 46,	13,
		12,	46,	11,	8,	46, 45, 46, 2,	42, 46,	2,	0,	46, 17,	46,	13,
		12,	46,	16,	46, 46, 46, 46, 46, 16, 46, 46,	46,	46, 16,	46,	13,
		12,	46,	46, 46, 3,	15, 1,	46,	46,	46,	3,	1,	46,	46,	46,	13,
		6,	15,	15,	15, 4,	47, 6,	15, 15, 15, 4,	6, 15, 15,	15,	4
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
		12,	46,	46, 46, 46, 46, 46, 46,	46,	46,	46,	46,	46,	46,	46,	13,
		12,	46,	11,	20,	20,	20,	20, 20, 20, 20, 20, 20, 20, 9,	46, 13,
		12,	46,	17,	46, 46, 46, 46, 13, 46, 46, 46, 46, 46, 17,	46, 13,
		12,	46,	17,	46, 46, 46, 46, 13, 46, 46, 46, 46, 46, 17,	46, 13,
		12,	46,	10, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 8,	46, 13,
		12,	46,	46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 46, 13,
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

	Game::centeringTheMap(currentMap);

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
					else
						pacman->setNextDirection(-1);

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);
	
					break;
				case sf::Keyboard::Right:
					if (pacman->direction() != Direction::RIGHT)
						pacman->setNextDirection(Direction::RIGHT);
					else
						pacman->setNextDirection(-1);

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);

					break;
				case sf::Keyboard::Up:
					if (pacman->direction() != Direction::UP)
						pacman->setNextDirection(Direction::UP);
					else
						pacman->setNextDirection(-1);

					if (pacman->isStandStill())
						pacman->setIsStandStill(false);

					break;
				case sf::Keyboard::Down:
					if (pacman->direction() != Direction::DOWN)
						pacman->setNextDirection(Direction::DOWN);
					else
						pacman->setNextDirection(-1);

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
		
		// Clean the window.
		window.clear();
		// Drawing level.
		currentMap->updateVertexArray();
		Game::centeringTheMap(currentMap);
		window.draw(*currentMap);
		// Drawing pacman.
		pacman->setPosition(Game::getGlobalPosition(pacman->coords(), game.currentLevel()));
		window.draw(*pacman);
		// Drawing ghosts.
		for (int i = 0; i < Game::GHOST_COUNT; i++) {
			Ghost* ghost = ghosts[i];
			ghost->setPosition(Game::getGlobalPosition(ghost->coords(), game.currentLevel()));
			window.draw(*(ghosts[i]));
		}
		// Drawing scoreText
		scoreText.setString(Game::convertIntToString(pacman->score, scoreText.getString().getSize()));
		window.draw(scoreText);

		window.display();
	}

	game.setIsWindowOpen(false);

	return 0;
}