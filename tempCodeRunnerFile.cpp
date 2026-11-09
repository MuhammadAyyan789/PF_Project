#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
int main()

{
	//creating a simple window

	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Game 1", sf::Style::Default);


	//defining a shape and its height and width
   //here 200 is the width of the rectangle
	//here 100 is the height of the rectangle
	//sf::RectangleShape rectangle({ 200,100 });

	sf::RectangleShape rectangle({200,100});     //for square named as circle

	rectangle.setFillColor(sf::Color::Red);

	rectangle.setPosition({ 300,300 });

	rectangle.setRotation(sf::degrees(30.f));

	//fill the  rectangel with a color
	//rectangle.setFillColor(sf::Color::Blue);

	//set position of the rectangle
	//rectangle.setPosition({300,300});

	while (window.isOpen())
	{

		while (std::optional event = window.pollEvent())
		{
			//closing a window only when a close button is pressed


			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{

				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					window.close();
				}
			}
		}

		window.clear();

		//window.draw(rectangle);

		window.draw(rectangle);

		window.display();
	}

}