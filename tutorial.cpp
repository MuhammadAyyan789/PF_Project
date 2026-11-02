#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>

int main()
{

	sf::RenderWindow window(sf::VideoMode({ 2000,1024 }), "Movement of Ship", sf::Style::Default);

	window.setFramerateLimit(350);

	//sf::RectangleShape rectangle({50,50});

	//rectangle.setFillColor(sf::Color::Red);

	//rectangle.setPosition({ 300,550 });

	sf::Texture texture;

	texture.loadFromFile("enemybattleship.png");



	sf::Texture back;

	back.loadFromFile("spaceback2.png");

	sf::Sprite background(back);

	//background.setScale({2.f,0.f});

	sf::Sprite battleship(texture);

	battleship.setTexture(texture);

	battleship.setPosition({ 400, 300 });


	battleship.setScale({ 0.1f, 0.1f });


	while (window.isOpen())
	{
		while (std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			if (battleship.getPosition().x < 1900)
			{
				battleship.move({ 1.f,0.f });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			if (battleship.getPosition().x > 0)
			{
				battleship.move({ -1.f,0.f });
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			if (battleship.getPosition().y > 0)
			{
				battleship.move({ 0,-1 });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			if (battleship.getPosition().y < 930)
			{
				battleship.move({ 0, 1 });
			}
		}


		window.clear(sf::Color::White);

		window.draw(background);

		window.draw(battleship);



		window.display();
	}

	return 0;
}