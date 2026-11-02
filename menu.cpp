//#include <SFML/Graphics.hpp>
//#include <iostream>
//
//int main()
//{
//    sf::RenderWindow window(sf::VideoMode({ 800, 600 }),"Menu");
//
//    sf::Font font;
//
//    if (!font.openFromFile("arial.ttf"))
//    {
//        std::cout << "FAILED TO LOAD FONT\n";
//        return 0;
//    }
//
//    std::cout << "FONT LOADED SUCCESSFULLY\n";
//
//    sf::Text title(font);
//
//    title.setString("Battleship Game");
// 
//    title.setCharacterSize(40);
// 
//    title.setFillColor(sf::Color::Blue);
// 
//    title.setStyle(sf::Text::Bold);
// 
//    title.setPosition({ 200.f, 300.f });
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
//        if (sf::Keyboard::isKeyPressed(
//            sf::Keyboard::Key::Escape))
//        {
//            window.close();
//        }
//
//        window.clear(sf::Color::Black);
//
//        window.draw(title);
//
//        window.display();
//    }
//}