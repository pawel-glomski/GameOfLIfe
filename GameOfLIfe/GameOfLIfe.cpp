#include <SFML/Graphics.hpp>
#include "GameOfCells.h"
#include <iostream>
#include <random>

int main()
{
	sf::Vector2u size;

	while (std::cout << "\nWidth of board: " && !(std::cin >> size.x))
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}
	std::cin.clear();
	std::cin.ignore(1000, '\n');
	while (std::cout << "\nHeight of board: " && !(std::cin >> size.y))
	{
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game of life", sf::Style::None);
	sf::View view(sf::Vector2f(size.x / 2, size.y / 2), sf::Vector2f(size));
	window.setView(view);
	window.setFramerateLimit(0);
	
	GameOfCells game(size);
	std::random_device device;

	sf::Event event;

	game.fillBoard(1);

	sf::Clock clock;
	float dt = 0;
	int fps = 120;

	while (window.isOpen())
	{
		dt = clock.getElapsedTime().asSeconds();

		while (window.pollEvent(event))	{}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			view.setCenter(view.getCenter().x, view.getCenter().y - 20);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			view.setCenter(view.getCenter().x, view.getCenter().y + 20);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			view.setCenter(view.getCenter().x - 20, view.getCenter().y);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			view.setCenter(view.getCenter().x + 20, view.getCenter().y);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
		{
			window.setFramerateLimit(++fps);
			std::cout << fps << std::endl;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))
		{
			window.setFramerateLimit(--fps);
			std::cout << fps << std::endl;
		}

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
			view.setSize(sf::Vector2f(view.getSize().x - 20, view.getSize().y - 20));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			view.setSize(sf::Vector2f(view.getSize().x + 20, view.getSize().y + 20));


		window.setView(view);

		game.simulate();
		window.clear();
		window.draw(game);
		window.display();
		std::cout << "fps: " << 1 / (clock.getElapsedTime().asSeconds() - dt) << std::endl;
	}
	
    return 0;
}