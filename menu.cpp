//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>
//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//#include <vector>
//#include <optional>
//
//struct ENemy
//{
//    sf::Sprite sprite;
//    float speed = 250.0f; // Pixels per second
//
//    ENemy(const sf::Texture& texture) : sprite(texture) {}
//};
//
//void SpawnEnemies(ENemy& enemy, float windowWidth)
//{
//    sf::FloatRect bounds = enemy.sprite.getGlobalBounds();
//
//    float enemyWidth = bounds.size.x;
//    float enemyHeight = bounds.size.y;
//
//    float max = windowWidth - enemyWidth;
//    if (max <= 1.0f) max = 1.0f;
//
//    float random = static_cast<float>(std::rand() % static_cast<int>(max));
//
//    enemy.sprite.setPosition({ random, -enemyHeight });
//}
//
//int main()
//{
//    std::srand(static_cast<unsigned int>(std::time(nullptr)));
//
//    sf::RenderWindow window(sf::VideoMode({ 2000, 1024 }), "Movement of Ship", sf::Style::Default);
//    window.setFramerateLimit(60);
//
//    // Player ship
//    sf::Image battleshipImage;
//    if (battleshipImage.loadFromFile("enemybattleship.png"))
//    {
//        battleshipImage.createMaskFromColor(sf::Color::White, 0);
//    }
//
//    sf::Texture texture;
//    (void)texture.loadFromImage(battleshipImage);
//
//    // Enemy ship
//    sf::Image obstacleImage;
//    if (obstacleImage.loadFromFile("Enemy_battleship.png"))
//    {
//        obstacleImage.createMaskFromColor(sf::Color::White, 0);
//    }
//
//    sf::Texture EnemyTexture;
//    (void)EnemyTexture.loadFromImage(obstacleImage);
//
//    ENemy enemy(EnemyTexture);
//    enemy.sprite.setScale({ 0.08f, 0.08f });
//
//    SpawnEnemies(enemy, 2000.0f);
//
//    // Background
//    sf::Texture back;
//    (void)back.loadFromFile("spaceback2.png");
//
//    sf::Sprite background(back);
//    background.setScale({ 1.0f, 1.5f });
//
//    // Player Sprite setup (placed inside visible screen)
//    sf::Sprite battleship(texture);
//    battleship.setPosition({ 900.f, 800.f });
//    battleship.setScale({ 0.1f, 0.1f });
//
//    // Background Audio
//    sf::Music Menu;
//    if (!Menu.openFromFile("back_music.mp3"))
//    {
//        std::cout << "Unable to load the menu background music\n";
//    }
//    else
//    {
//        Menu.setLooping(true);
//        Menu.setVolume(50.f);
//        Menu.play();
//    }
//
//    // Font & Text
//    sf::Font F;
//    if (!F.openFromFile("arial.ttf"))
//    {
//        std::cout << "Failed to load the text style\n";
//    }
//
//    sf::Text text(F, "Welcome to the world of disaster", 70);
//    text.setStyle(sf::Text::Bold | sf::Text::Italic);
//    text.setFillColor(sf::Color::Blue);
//    text.setPosition({ 500.f, 300.f });
//
//    sf::Texture backimage;
//    if (!backimage.loadFromFile("menu_image.jpg"))
//    {
//        std::cout << "Failed to load background image 'menu_image.jpg'\n";
//        return -1;
//    }
//
//    sf::Sprite backs(backimage);
//    backs.setScale({ 1.6f, 1.5f });
//
//    sf::Text startButton(F, "PLAY", 50);
//    startButton.setPosition({ 900.f, 650.f });
//    startButton.setFillColor(sf::Color::Cyan);
//
//    sf::Text exitButton(F, "EXIT", 50);
//    exitButton.setPosition({ 900.f, 750.f });
//    exitButton.setFillColor(sf::Color::Cyan);
//
//    enum Gamestate
//    {
//        MENU,
//        PLAYING
//    };
//
//    Gamestate currentstate = MENU;
//
//    sf::Clock clock;
//    float speed = 400.f;
//
//    // Bullet resources
//    sf::Image bullet;
//    bullet.resize({ 8, 20 }, sf::Color::Yellow);
//    sf::Texture bullettexture;
//    (void)bullettexture.loadFromImage(bullet);
//    std::vector<sf::Sprite> bullets;
//    sf::Clock shoot;
//
//    while (window.isOpen())
//    {
//        float dt = clock.restart().asSeconds();
//
//        while (std::optional event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//            {
//                window.close();
//            }
//
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
//            {
//                window.close();
//            }
//
//            // Fixed mouse input
//            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
//            {
//                sf::Vector2f mousePos(static_cast<float>(mousePressed->position.x), static_cast<float>(mousePressed->position.y));
//                if (currentstate == Gamestate::MENU)
//                {
//                    if (startButton.getGlobalBounds().contains(mousePos))
//                    {
//                        currentstate = Gamestate::PLAYING;
//                        Menu.stop();
//                    }
//
//                    if (exitButton.getGlobalBounds().contains(mousePos))
//                    {
//                        window.close();
//                    }
//                }
//            }
//        }
//
//        if (currentstate == Gamestate::PLAYING)
//        {
//            // Player movement
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
//            {
//                if (battleship.getPosition().x < 2000.f - battleship.getGlobalBounds().size.x)
//                {
//                    battleship.move({ speed * dt, 0.f });
//                }
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
//            {
//                if (battleship.getPosition().x > 0.f)
//                {
//                    battleship.move({ -speed * dt, 0.f });
//                }
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
//            {
//                if (battleship.getPosition().y > 0.f)
//                {
//                    battleship.move({ 0.f, -speed * dt });
//                }
//            }
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
//            {
//                if (battleship.getPosition().y < 1024.f - battleship.getGlobalBounds().size.y)
//                {
//                    battleship.move({ 0.f, speed * dt });
//                }
//            }
//
//            // Firing
//            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
//            {
//                if (shoot.getElapsedTime().asSeconds() > 0.15f)
//                {
//                    sf::Sprite newBullet(bullettexture);
//                    float bulletx = battleship.getPosition().x + (battleship.getGlobalBounds().size.x / 2.f) - 4.f;
//                    float bullety = battleship.getPosition().y;
//
//                    newBullet.setPosition({ bulletx, bullety });
//                    bullets.push_back(newBullet);
//
//                    shoot.restart();
//                }
//            }
//
//            // Bullet movement
//            for (auto it = bullets.begin(); it != bullets.end(); )
//            {
//                it->move({ 0.f, -600.f * dt });
//
//                if (it->getPosition().y < 0.f)
//                {
//                    it = bullets.erase(it);
//                }
//                else
//                {
//                    ++it;
//                }
//            }
//
//            // Enemy movement using delta time
//            enemy.sprite.move({ 0.f, enemy.speed * dt });
//
//            // Respawn if enemy moves below bottom of the screen
//            if (enemy.sprite.getPosition().y > 1024.0f)
//            {
//                SpawnEnemies(enemy, 2000.0f);
//            }
//        }
//
//        window.clear(sf::Color::Black);
//
//        if (currentstate == Gamestate::MENU)
//        {
//            window.draw(backs);
//            window.draw(text);
//            window.draw(startButton);
//            window.draw(exitButton);
//        }
//        else if (currentstate == Gamestate::PLAYING)
//        {
//            window.draw(background);
//            window.draw(battleship);
//            window.draw(enemy.sprite);
//            for (const auto& b : bullets)
//            {
//                window.draw(b);
//            }
//        }
//
//        window.display();
//    }
//
//    return 0;
//}