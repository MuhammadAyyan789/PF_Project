//#include <SFML/Graphics.hpp>
//#include <SFML/Audio.hpp>
//#include <SFML/Network.hpp>
//#include <iostream>
//#include <cstdlib>
//#include <ctime>
//
//struct ENemy
//{
//    sf::Sprite sprite;
//    float speed = 1.0f;
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
//    window.setFramerateLimit(350);
//
//    sf::Image battleshipImage;
//    if (battleshipImage.loadFromFile("enemybattleship.png"))
//    {
//        battleshipImage.createMaskFromColor(sf::Color::White, 0);
//        battleshipImage.createMaskFromColor(sf::Color::Transparent, 0);
//    }
//
//    sf::Texture texture;
//    texture.loadFromImage(battleshipImage);
//
//    sf::Image obstacleImage;
//    if (obstacleImage.loadFromFile("Enemy_battleship.png"))
//    {
//        obstacleImage.createMaskFromColor(sf::Color::Transparent, 0);
//    }
//
//    sf::Texture EnemyTexture;
//    EnemyTexture.loadFromImage(obstacleImage);
//
//    ENemy enemy(EnemyTexture);
//    enemy.sprite.setScale({ 0.08f, 0.08f });
//
//    SpawnEnemies(enemy, 2000.0f);
//
//    sf::Texture back;
//    back.loadFromFile("spaceback2.png");
//
//    sf::Sprite background(back);
//
//    sf::Sprite battleship(texture);
//    battleship.setPosition({ 600.f, 930.f });
//    battleship.setScale({ 0.1f, 0.1f });
//
//    while (window.isOpen())
//    {
//        while (std::optional event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//            {
//                window.close();
//            }
//        }
//
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
//        {
//            if (battleship.getPosition().x < 1900)
//            {
//                battleship.move({ 1.f, 0.f });
//            }
//        }
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
//        {
//            if (battleship.getPosition().x > 0)
//            {
//                battleship.move({ -1.f, 0.f });
//            }
//        }
//
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
//        {
//            if (battleship.getPosition().y > 0)
//            {
//                battleship.move({ 0.f, -1.f });
//            }
//        }
//        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
//        {
//            if (battleship.getPosition().y < 930)
//            {
//                battleship.move({ 0.f, 1.f });
//            }
//        }
//
// 
//        enemy.sprite.move({ 0.f, enemy.speed });
//
//        if (enemy.sprite.getPosition().y > 500.0f)
//        {
//            SpawnEnemies(enemy, 2000.0f);
//        }
//
//        window.clear(sf::Color::White);
//
//        window.draw(background);
//        window.draw(battleship);
//        window.draw(enemy.sprite);
//
//        window.display();
//    }
//
//    return 0;
//}