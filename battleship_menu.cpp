#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

struct ENemy
{
    sf::Sprite sprite;
    float speed = 1.0f;

    ENemy(const sf::Texture& texture) : sprite(texture) {}
};

void SpawnEnemies(ENemy& enemy, float windowWidth)
{
    sf::FloatRect bounds = enemy.sprite.getGlobalBounds();

    float enemyWidth = bounds.size.x;
    float enemyHeight = bounds.size.y;

    float max = windowWidth - enemyWidth;

    float random = static_cast<float>(std::rand() % static_cast<int>(max));

    enemy.sprite.setPosition({ random, -enemyHeight });
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({ 2000, 1600 }), "Movement of Ship", sf::Style::Default);
    window.setFramerateLimit(350);

    sf::Image battleshipImage;
    if (battleshipImage.loadFromFile("enemybattleship.png"))
    {
        battleshipImage.createMaskFromColor(sf::Color::White, 0);
        battleshipImage.createMaskFromColor(sf::Color::Transparent, 0);
    }

    sf::Texture texture;
    texture.loadFromImage(battleshipImage);

    sf::Image obstacleImage;
    if (obstacleImage.loadFromFile("Enemy_battleship.png"))
    {
        obstacleImage.createMaskFromColor(sf::Color::Transparent, 0);
    }

    sf::Texture EnemyTexture;
    EnemyTexture.loadFromImage(obstacleImage);

    ENemy enemy(EnemyTexture);
    enemy.sprite.setScale({ 0.35f, 0.35f });

    SpawnEnemies(enemy, 2000.0f);

    sf::Texture back;
    back.loadFromFile("spaceback2.png");   //back is the background of the battleship playing game state

    sf::Sprite background(back);
    background.setScale({ 1.0f,1.5f });

    sf::Sprite battleship(texture);
    battleship.setTexture(texture);
    battleship.setPosition({ 900, 1200 });
    battleship.setScale({ 0.4f, 0.4f });

    sf::Music Menu;

    if (!(Menu.openFromFile("back_music.mp3")))
    {
       std::cout << "Unable to load the menu background music";
    }
   else
   {
    Menu.setLooping(true);
    Menu.setVolume(100.f);
    Menu.play();
   }

    sf::Font F;
    if (!(F.openFromFile("arial.ttf")))
    {
        std::cout << "Failed to load the text style ";
    }

    sf::Text text(F, "Welcome to the world of disaster", 70);

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

    sf::Text startButton(F, "PLAY", 50);
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

    sf::Image bullet;
    bullet.resize({ 8, 20 }, sf::Color::Yellow);
    sf::Texture bullettexture;
    (void)bullettexture.loadFromImage(bullet);
    std::vector<sf::Sprite> bullets;
    sf::Clock shoot;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
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
                        Menu.stop();
                    }

                    if (exitButton.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                    }
                }
            }
        }


        if (currentstate == Gamestate::PLAYING)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                if (battleship.getPosition().x < 2000)
                {
                    battleship.move({ speed * dt, 0.f });
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                if (battleship.getPosition().x > 0)
                {
                    battleship.move({ -speed * dt, 0.f });
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                if (battleship.getPosition().y > 0)
                {
                    battleship.move({ 0.f, -speed * dt });
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                if (battleship.getPosition().y < 1350)
                {
                    battleship.move({ 0.f, speed * dt });
                }
            }

            // firing......
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                if (shoot.getElapsedTime().asSeconds() > 0.15f)
                {
                    sf::Sprite newBullet(bullettexture);
                    float bulletx = battleship.getPosition().x + (battleship.getGlobalBounds().size.x / 2.f) - 4.f;
                    float bullety = battleship.getPosition().y;

                    newBullet.setPosition({ bulletx, bullety });
                    bullets.push_back(newBullet);

                    shoot.restart();
                }
            }

            // movement of bullet
            for (auto it = bullets.begin(); it != bullets.end(); )
            {
                it->move({ 0.f, -500.f * dt });

                if (it->getPosition().y < 0)
                {
                    it = bullets.erase(it);
                }
                else
                {
                    it++;
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
            window.draw(background);
            window.draw(battleship);
            window.draw(enemy.sprite);
            for (const auto& b : bullets)
            {
                window.draw(b);
            }


            enemy.sprite.move({ 0.f, enemy.speed });

            if (enemy.sprite.getPosition().y > 1500.0f)
            {
                SpawnEnemies(enemy, 2000.0f);
            }
        }

        window.display();
    }

    return 0;
}