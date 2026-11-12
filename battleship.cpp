#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <optional>
#include <fstream>

struct ENemy
{
    sf::Sprite sprite;
    float speed = 150.0f; // Pixels per second

    ENemy(const sf::Texture& texture) : sprite(texture) {};
};

void SpawnEnemies(ENemy& enemy, float windowWidth)
{
    sf::FloatRect bounds = enemy.sprite.getGlobalBounds();

    float enemyWidth = bounds.size.x;
    float enemyHeight = bounds.size.y;

    float max = windowWidth - enemyWidth;
    if (max <= 1.0f) max = 1.0f;

    float random = static_cast<float>(std::rand() % static_cast<int>(max));

    enemy.sprite.setPosition({ random, -enemyHeight });
}

int loadHighest()
{
    //initially load the highest value from the file to highest
    int highest = 0;
    const std::string filename("high.txt");

    std::ifstream FileIn(filename);

    if (FileIn.is_open())
    {
        FileIn >> highest;
        FileIn.close();
    }

    return highest;
}
int UpdatedHighestvalue(int current, int& high)
{
    //modify the existing highest value in the file

    if (current > high)
    {
        high = current;
        const std::string filename("high.txt");

        std::ofstream FileOut(filename);
        if (FileOut.is_open())
        {
            FileOut << high;
            FileOut.close();
        }
    }

    return high;
}

int main()
{

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({ 2000, 1600 }), "Movement of Ship", sf::Style::Default);
    window.setFramerateLimit(60);

    // Player ship
    sf::Image battleshipImage;
    if (battleshipImage.loadFromFile("enemybattleship.png"))
    {
        battleshipImage.createMaskFromColor(sf::Color::White, 0);
    }

    sf::Texture texture;
    (void)texture.loadFromImage(battleshipImage);

    // Enemy ship
    sf::Image obstacleImage;
    if (obstacleImage.loadFromFile("Enemy_battleship.png"))
    {
        obstacleImage.createMaskFromColor(sf::Color::White, 0);
    }

    sf::Texture EnemyTexture;
    (void)EnemyTexture.loadFromImage(obstacleImage);

    ENemy enemy(EnemyTexture);
    enemy.sprite.setScale({ 0.35f, 0.35f });

    SpawnEnemies(enemy, 2000.0f);

    // Background
    sf::Texture back;
    (void)back.loadFromFile("spaceback2.png");

    sf::Sprite background(back);
    background.setScale({ 1.0f, 1.5f });

    // Player Sprite setup (placed inside visible screen)
    sf::Sprite battleship(texture);
    battleship.setPosition({ 900.f, 1200.f });
    battleship.setScale({ 0.4f, 0.4f });

    //Selection menu back image
    sf::Texture select;
    if(!select.loadFromFile("selection.jpeg"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Sprite Image(select);
    (void)Image.setScale({ 1.5f,1.9f });

        //GameOver
    sf::Texture over;
    if(!over.loadFromFile("GameOver1.jpg"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Sprite End(over);
    End.setScale({1.9f,1.7f});

    // Background Audio
    sf::Music Menu;
    if (!Menu.openFromFile("back_music.mp3"))
    {
        std::cout << "Unable to load the menu background music\n";
    }
    else
    {
        Menu.setLooping(true);
        Menu.setVolume(100.f);
        Menu.play();
    }

    sf::Music Game;
    if (!Game.openFromFile("gameMusic.mp3"))
    {
        std::cout << "Unable to load the game music\n";
    }
    else
    {
        Game.setLooping(true);
        Game.setVolume(50.f);
    }

    //bullets buffer voice
    sf::SoundBuffer bulletsound;
    if (!bulletsound.loadFromFile("laserGun.mp3"))
    {
        std::cout << "Unable to load buffer ";
    }
    sf::Sound sound(bulletsound);
    sound.setVolume(100);


    // Font & Text
    sf::Font F;
    if (!F.openFromFile("Style.ttf"))
    {
        std::cout << "Failed to load the text style\n";
    }

    sf::Font F1;
    if(!F1.openFromFile("style1.ttf"))
    {
        std::cout<<"Failes to load the text style\n";
    }
    // sf::Text text(F, "Welcome to the world of disaster", 70);
    // text.setStyle(sf::Text::Bold | sf::Text::Italic);
    // text.setFillColor(sf::Color::Red);
    // text.setPosition({ 500.f, 200.f });

    sf::Text selectmenu(F, "SELECT YOUR BATTLESHIP", 70);
    selectmenu.setStyle(sf::Text::Bold);
    selectmenu.setFillColor(sf::Color::Blue);
    selectmenu.setPosition({ 550.f,250 });

    //score
    int totalscore = 0;
    sf::Text scoreText(F1, "Score: 0", 30);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition({ 50.f, 200.f });
    scoreText.setStyle(sf::Text::Bold | sf::Text::Italic);


    sf::Texture backimage;
    if (!backimage.loadFromFile("menu2.jpeg"))
    {
        std::cout << "Failed to load background image 'menu_image.jpg'\n";
        return -1;
    }

    //images of background
    sf::Sprite backs(backimage);
    backs.setScale({ 1.6f, 1.8f });

    //Startbutton
    sf::RectangleShape rectangle({ 200,80 });
    rectangle.setPosition({ 870,890 });
    rectangle.setFillColor(sf::Color(40, 37, 68));

    sf::Text startButton(F, "PLAY", 50);
    startButton.setPosition({ 915.f, 900.f });
    startButton.setFillColor(sf::Color::Cyan);

    //Selection Menu
    sf::RectangleShape rectangle1({ 600,80 });
    rectangle1.setPosition({ 670,990 });
    rectangle1.setFillColor(sf::Color(40, 37, 68));

    sf::Text Settings(F, "BATTLESHIP SELECTION", 50);
    Settings.setPosition({ 730.f,1000.f });
    Settings.setFillColor(sf::Color::Cyan);

    //ExitButton
    sf::RectangleShape rectangle2({ 200,80 });
    rectangle2.setPosition({ 870,1090 });
    rectangle2.setFillColor(sf::Color(40, 37, 68));

    sf::Text exitButton(F, "EXIT", 50);
    exitButton.setPosition({ 915.f, 1100.f });
    exitButton.setFillColor(sf::Color::Cyan);

    //Return to Main Menu Option
    sf::Text backOption(F, "Return", 30);
    backOption.setFillColor(sf::Color::Black);
    backOption.setPosition({ 1750,200 });

    sf::RectangleShape returnBox({ 120,50 });
    returnBox.setPosition({ 1740,185 });
    returnBox.setFillColor(sf::Color::Red);

    //selection state
    sf::RectangleShape box1({450,800});
    box1.setPosition({300,400});
    box1.setFillColor(sf::Color(12,18,28,210));

    sf::RectangleShape box2({450,800});
    box2.setPosition({800,400});
    box2.setFillColor(sf::Color(12,18,28,210));

    sf::RectangleShape box3({450,800});
    box3.setPosition({1300,400});
    box3.setFillColor(sf::Color(12,18,28,210));

    //Options
    // sf::RectangleShape subbox1({420,400});
    // subbox1.setPosition({310,410});
    // subbox1.setFillColor(sf::Color(40,37,68));

    sf::Sprite option1(texture);
    option1.setScale({1.0f,1.0f});
    option1.setPosition({200,420});
    enum Gamestate
    {
        MENU,
        PLAYING,
        GAME_OVER,
        SETTINGS
    };

    Gamestate currentstate = MENU;

    sf::Clock clock;
    float speed = 300.f;


    // Bullet resources
    sf::Image bullet;
    bullet.resize({ 8, 20 }, sf::Color::Yellow);
    sf::Texture bullettexture;
    (void)bullettexture.loadFromImage(bullet);
    std::vector<sf::Sprite> bullets;
    sf::Clock shoot;

    sf::Image enemybullet;
    enemybullet.resize({ 8, 20 }, sf::Color::Yellow);
    sf::Texture enemybullettexture;
    (void)enemybullettexture.loadFromImage(enemybullet);
    std::vector<sf::Sprite> enemybullets;
    sf::Clock enemyshoot;

    //highest score
    int highest_score = loadHighest();

    sf::Text high(F1, "Highest Score:0", 30);
    high.setFillColor(sf::Color::Magenta);
    high.setPosition({ 50.f,260.f });
    high.setStyle(sf::Text::Bold | sf::Text::Italic);


    //health part
    int health = 200;

    sf::Text healthy(F1,"Health : 200 ",30);
    healthy.setFillColor(sf::Color(255,130,20));
    healthy.setPosition({50.f,320.f});
    healthy.setStyle(sf::Text::Bold | sf::Text::Italic);

    float enemy_bullet_speed = 300.f;


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

            // Fixed mouse input
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
                if (currentstate == Gamestate::MENU)
                {
                    if (startButton.getGlobalBounds().contains(mousePos))
                    {
                        currentstate = Gamestate::PLAYING;
                        Menu.stop();
                        Game.play();
                    }

                    if (Settings.getGlobalBounds().contains(mousePos))
                    {
                        currentstate = Gamestate::SETTINGS;
                        Game.stop();

                    }

                    if (exitButton.getGlobalBounds().contains(mousePos))
                    {
                        window.close();
                    }
                }

                if (currentstate == Gamestate::PLAYING)
                {
                    if (backOption.getGlobalBounds().contains(mousePos))
                    {
                        currentstate = Gamestate::MENU;
                        Menu.play();
                        Game.stop();
                        enemybullets.clear();
                    }
                }

                if (currentstate == Gamestate::SETTINGS)
                {
                    if (backOption.getGlobalBounds().contains(mousePos))
                    {
                        currentstate = Gamestate::MENU;
                        Menu.play();
                    }
                }

            }
        }

        if (currentstate == Gamestate::PLAYING)
        {
            // Player movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                if (battleship.getPosition().x < 2000.f - battleship.getGlobalBounds().size.x)
                {
                    battleship.move({ speed * dt, 0.f });
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                if (battleship.getPosition().x > 0.f)
                {
                    battleship.move({ -speed * dt, 0.f });
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
            {
                if (battleship.getPosition().y > 200.f)
                {
                    battleship.move({ 0.f, -speed * dt });
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                if (battleship.getPosition().y < 1400.f - battleship.getGlobalBounds().size.y)
                {
                    battleship.move({ 0.f, speed * dt });
                }
            }

            // Firing
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                if (shoot.getElapsedTime().asSeconds() > 0.15f)
                {
                    sf::Sprite newBullet(bullettexture);
                    float bulletx = battleship.getPosition().x + (battleship.getGlobalBounds().size.x / 2.f) - 4.f;
                    float bullety = battleship.getPosition().y;

                    newBullet.setPosition({ bulletx, bullety });
                    bullets.push_back(newBullet);
                    sound.play();


                    shoot.restart();
                }
            }

            // Bullet movement
            for (auto it = bullets.begin(); it != bullets.end(); )
            {
                it->move({ 0.f, -600.f * dt });
                // it will check if bullet hit enemy 
                if (it->getGlobalBounds().findIntersection(enemy.sprite.getGlobalBounds()))
                {
                    // score increment
                    totalscore += 20;
                    scoreText.setString("Score: " + std::to_string(totalscore));

                    int highest = UpdatedHighestvalue(totalscore, highest_score);

                    high.setString("Highest Score: " + std::to_string(highest));
                    SpawnEnemies(enemy, 2000.0f);
                    it = bullets.erase(it);
                }
                else if (it->getPosition().y < 0.f)
                {
                    it = bullets.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            // Enemy movement using delta time
            enemy.sprite.move({ 0.f, enemy.speed * dt });

            // Respawn if enemy moves below bottom of the screen
            if (enemy.sprite.getPosition().y > 1400.0f)
            {
                SpawnEnemies(enemy, 2000.0f);
                enemybullets.clear();
            }
            if (enemyshoot.getElapsedTime().asSeconds() > 1.2f)
            {
                if (enemy.sprite.getPosition().y > 0.0f)
                {
                    sf::Sprite eBullet(enemybullettexture);
                    float eBulletX = enemy.sprite.getPosition().x + (enemy.sprite.getGlobalBounds().size.x / 2.f) - 4.f;
                    float eBulletY = enemy.sprite.getPosition().y + enemy.sprite.getGlobalBounds().size.y;

                    eBullet.setPosition({ eBulletX, eBulletY });
                    enemybullets.push_back(eBullet);
                }
                enemyshoot.restart();
            }
            for (auto it = enemybullets.begin(); it != enemybullets.end(); )
            {
                it->move({ 0.f, enemy_bullet_speed * dt });

                if (it->getGlobalBounds().findIntersection(battleship.getGlobalBounds()))
                {
                    it = enemybullets.erase(it);
                    // enemybullets.clear();
                    // bullets.clear();
                    // currentstate = Gamestate::MENU;
                    // Menu.play();
                    // Game.stop();
                    // break;
                    health = health-10;
                    healthy.setString("Health : " + std::to_string(health));
                    if(health<=0)
                    {
                        currentstate = Gamestate::GAME_OVER;
                        Game.stop();
                    }
                }
                else if (it->getPosition().y > 1600.f)
                {
                    it = enemybullets.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            if (totalscore % 100 == 0)
            {
                enemy.speed += 0.2;
                enemy_bullet_speed += 0.2;
            }

            if (totalscore % 200 == 0)
            {
                speed += 0.2;
            }
        }

        window.clear(sf::Color::Black);

        if (currentstate == Gamestate::MENU)
        {
            window.draw(backs);
            // window.draw(text);
            window.draw(rectangle);
            window.draw(startButton);
            window.draw(rectangle1);
            window.draw(Settings);
            window.draw(rectangle2);
            window.draw(exitButton);
        }
        else if (currentstate == Gamestate::PLAYING)
        {
            window.draw(background);
            window.draw(returnBox);
            window.draw(backOption);
            window.draw(battleship);
            window.draw(enemy.sprite);
            for (const auto& b : bullets)
            {
                window.draw(b);
            }
            for (const auto& eb : enemybullets)
            {
                window.draw(eb);
            }
            window.draw(scoreText);
            window.draw(high);
            window.draw(healthy);
        }
        else if (currentstate == Gamestate::SETTINGS)
        {
            window.draw(Image);
            window.draw(box1);
            window.draw(box2);
            window.draw(box3);
            window.draw(option1);
            window.draw(returnBox);
            window.draw(backOption);
            window.draw(selectmenu);
        }
        else if(currentstate == Gamestate::GAME_OVER)
        {
            window.draw(End);
        }

        window.display();
    }

    return 0;
}