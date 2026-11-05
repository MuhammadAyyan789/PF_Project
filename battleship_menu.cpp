#include<iostream>
#include<SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 2000,1600 }), "Battleship Menu");

    window.setFramerateLimit(30);


    sf::Texture texture;

    texture.loadFromFile("enemybattleship.png");



    sf::Texture back;

    back.loadFromFile("spaceback2.png");   //back is the background of the battleship playing game state

    sf::Sprite background(back);

    background.setScale({1.0f,1.5f});

    sf::Sprite battleship(texture);

    battleship.setTexture(texture);

    battleship.setPosition({ 900, 1200 });


    battleship.setScale({ 0.1f, 0.1f });

    sf::Font F;

	if (!(F.openFromFile("arial.ttf")))
	{
		std::cout << "Failed to load the text style ";
	}

	//std::cout << "Successfully loaded the text style";

	sf::Text text(F,"Welcome to the world of disaster",70);

    sf::Texture backimage;

    if (!backimage.loadFromFile("menu_image.jpg"))
    {
        std::cout << "Failed to load background image 'menu_image.jpg'\n";
        return -1;
    }

    sf::Sprite backs(backimage);    //backs is the background of the battleship

    backs.setTexture(backimage);

    backs.setScale({ 1.6f,1.5f });

    text.setStyle(sf::Text::Bold);

    text.setStyle(sf::Text::Italic);

	text.setFillColor(sf::Color::Blue);

	text.setPosition({ 500,300 });

    sf::Text startButton(F,"PLAY", 50);

    startButton.setPosition({ 900, 850 });
    startButton.setFillColor(sf::Color::Cyan);

    sf::Text exitButton(F, "EXIT", 50);

    exitButton.setPosition({ 900,950 });
    exitButton.setFillColor(sf::Color::Cyan);

    enum Gamestate
    {
        MENU,
        PLAYING
    };

    Gamestate currentstate = MENU;

    sf::Clock clock;
    float speed = 300.f;

    //sf::Text GameStart(F, "Game has just started", 70);

    //GameStart.setFillColor(sf::Color::Red);
    //GameStart.setPosition({ 690,700 });

	    while (window.isOpen())
    {
            float dt = clock.restart().asSeconds();
            while (std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }


                if (sf::Keyboard::isKeyPressed(
                    sf::Keyboard::Key::Escape))
                {
                    window.close();
                }

                if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
                    if (currentstate == Gamestate::MENU)
                    {
                        if (startButton.getGlobalBounds().contains(mousePos))
                        {
                            currentstate = Gamestate::PLAYING;
                        }
                        if (exitButton.getGlobalBounds().contains(mousePos))
                        {
                            window.close();
                        }
                    }
                }
                if (currentstate == Gamestate::PLAYING)
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                    {
                        if (battleship.getPosition().x < 1850)
                        {
                            battleship.move({ speed*dt,0.f });
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                    {
                        if (battleship.getPosition().x > 30.f)
                        {
                            battleship.move({ -speed*dt,0.f });
                        }
                    }

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                    {
                        if (battleship.getPosition().y > 140.f)
                        {
                            battleship.move({ 0.f,-speed*dt });
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                    {
                        if (battleship.getPosition().y < 1250)
                        {
                            battleship.move({ 0.f, speed*dt });
                        }
                    }
                }

                
            }

            window.clear(sf::Color::Black);

        if (currentstate == Gamestate::MENU)
        {
            window.draw(backs);
            window.draw(text);
            window.draw(startButton);
            window.draw(exitButton);
        }

        if (currentstate == Gamestate::PLAYING)
        {
            //window.clear(sf::Color::Black);
            //window.draw(GameStart);
            window.draw(background);
            window.draw(battleship);
        }

        window.display();
    }
}
