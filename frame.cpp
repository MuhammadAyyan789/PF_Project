#include<SFML/Graphics.hpp>
#include<iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 2000, 1600 }), "Movement of Ship", sf::Style::Default);

    sf::Image car;
    if(!car.loadFromFile("car_png.png"))
    {
        std::cout<<"failed to load";
    }
    
    car.createMaskFromColor(sf::Color::White,0);

    sf::Texture car_p;
    car_p.loadFromImage(car);

    sf::Sprite carPicture(car_p);

    // Main game loop
    while (window.isOpen())
    {
        // Event processing...
        
        // Render pipeline
        window.clear();
        
        // window.draw(yourSprite);
        
        window.draw(carPicture);
        window.display();
    }

    return 0;

}