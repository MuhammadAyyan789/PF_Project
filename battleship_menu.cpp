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

    sf::Clock shootClock;
    float shootinterval = 1.5f;

    ENemy(const sf::Texture& texture) : sprite(texture)
    {
        sprite.setScale({ 0.35f, 0.35f });
        shootinterval = 1.0f + static_cast<float>(std::rand() % 15) / 10.0f;
    };
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
void Spawnnew(std::vector<ENemy>& enemies, const sf::Texture& texture, float windowWidth)
{
    ENemy newenemy(texture);
    sf::FloatRect bounds = newenemy.sprite.getGlobalBounds();

    float enemywidth = bounds.size.x;
    float enemyheight = bounds.size.y;

    float max = windowWidth - enemywidth;
    if (max <= 1.0f) max = 1.0f;

    float randomX = static_cast<float>(std::rand() % static_cast<int>(max));

    newenemy.sprite.setPosition({ randomX, -enemyheight - 10.f });
    enemies.push_back(newenemy);
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

    sf::RenderWindow window(sf::VideoMode({ 2000, 1300 }), "Movement of Ship", sf::Style::Default);
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

    std::vector<ENemy> enemies;
    sf::Clock enemySpawnClock;
    
    // Background
    sf::Texture back;
    (void)back.loadFromFile("spaceback2.png");

    sf::Sprite background(back);
    background.setScale({ 1.0f, 1.5f });

    // Player Sprite setup (placed inside visible screen)
    sf::Sprite battleship(texture);
    battleship.setScale({0.4,0.4});
    battleship.setPosition({ 900.f, 1200.f });

    //Selection menu back image
    sf::Texture select;
    if(!select.loadFromFile("selection.jpeg"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Sprite Image(select);
    (void)Image.setScale({ 1.0f,1.0f });

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
    selectmenu.setFillColor(sf::Color::White);
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
    }

    //images of background
    sf::Sprite backs(backimage);
    backs.setScale({ 1.6f, 2.2f });

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
    box1.setFillColor(sf::Color(40, 37, 68));

    sf::RectangleShape box2({450,800});
    box2.setPosition({800,400});
    box2.setFillColor(sf::Color(40, 37, 68));

    sf::RectangleShape box3({450,800});
    box3.setPosition({1300,400});
    box3.setFillColor(sf::Color(40, 37, 68));



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

           // scale window to fit the monitor so ui elements are not cut off
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    if (desktop.size.x < 2050 || desktop.size.y < 1680)
    {
        float scaleX = static_cast<float>(desktop.size.x - 100) / 2000.f;
        float scaleY = static_cast<float>(desktop.size.y - 140) / 1300.f;
        float scale = scaleY;
        if (scaleX < scaleY)
        {
            scale = scaleX;
        }
        if (scale < 0.3f) scale = 0.3f;

        unsigned int newW = static_cast<unsigned int>(2000.f * scale);
        unsigned int newH = static_cast<unsigned int>(1600.f * scale);
        window.setSize({ newW, newH });

        int posX = (static_cast<int>(desktop.size.x) - static_cast<int>(newW)) / 2;
        int posY = (static_cast<int>(desktop.size.y) - static_cast<int>(newH)) / 2 - 20;
        if (posX < 0) posX = 0;
        if (posY < 0) posY = 0;
        window.setPosition({ posX, posY });
    }
    sf::View gameView(sf::FloatRect({ 0.f, 0.f }, { 2000.f, 1600.f }));
    window.setView(gameView);

    // subscreen tracker 0 = choose battleship 1 = choose biome, 2 = tutoria
    int customScreen = 0;
    int selectedShip = 1;
    int selectedBiome = 1;
    bool isMuted = false;
    bool menuMusicActive = true;
    bool gameMusicActive = false;
    Gamestate prevState = MENU;

    //  other 2 battleship textures
    sf::Image shipImg2;
    if (!shipImg2.loadFromFile("battleship2.png"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Texture shipTex2;
    (void)shipTex2.loadFromImage(shipImg2);

    sf::Image shipImg3;
    if (!shipImg3.loadFromFile("battleship3.png"))
    {
        std::cout<<"Unable to load image";
    }
    
    sf::Texture shipTex3;
    (void)shipTex3.loadFromImage(shipImg3);

    //  Load 2 other biome background textures (itll go back to default if other files arent added)
    sf::Texture biomeTex2;
    if (!biomeTex2.loadFromFile("biome2.png"))
        (void)biomeTex2.loadFromFile("spaceback2.png");

    sf::Texture biomeTex3;
    if (!biomeTex3.loadFromFile("biome3.png"))
        (void)biomeTex3.loadFromFile("spaceback2.png");

    //  choose biome button 
    sf::RectangleShape biomeBox({ 430.f, 80.f });
    biomeBox.setPosition({ 765.f, 1090.f });
    biomeBox.setFillColor(sf::Color(40, 37, 68));

    sf::Text biomeButton(F, "CHOOSE BIOME", 50);
    biomeButton.setPosition({ 790.f, 1100.f });
    biomeButton.setFillColor(sf::Color::Cyan);

    // how to play button
    sf::RectangleShape howToPlayBox({ 390.f, 80.f });
    howToPlayBox.setPosition({ 785.f, 1190.f });
    howToPlayBox.setFillColor(sf::Color(40, 37, 68));

    sf::Text howToPlayButton(F, "HOW TO PLAY", 50);
    howToPlayButton.setPosition({ 810.f, 1200.f });
    howToPlayButton.setFillColor(sf::Color::Cyan);

    // exit button to the last
    rectangle2.setPosition({ 870.f, 1290.f });
    exitButton.setPosition({ 900.f, 1300.f });

    // sound mute unmute
    sf::RectangleShape muteBox({ 260.f, 65.f });
    muteBox.setPosition({ 50.f, 50.f });
    muteBox.setFillColor(sf::Color::Black);
    muteBox.setOutlineThickness(4.f);
    muteBox.setOutlineColor(sf::Color::Cyan);

    sf::Text muteButton(F, "SOUND: ON", 32);
    muteButton.setPosition({ 78.f, 62.f });
    muteButton.setFillColor(sf::Color::Cyan);

    // battleship selection cards 
    sf::RectangleShape shipCard1({ 420.f, 480.f });
    shipCard1.setPosition({ 250.f, 450.f });
    shipCard1.setFillColor(sf::Color(30, 30, 30));
    shipCard1.setOutlineThickness(6.f);

    sf::RectangleShape shipCard2({ 420.f, 480.f });
    shipCard2.setPosition({ 790.f, 450.f });
    shipCard2.setFillColor(sf::Color(30, 30, 30));
    shipCard2.setOutlineThickness(6.f);

    sf::RectangleShape shipCard3({ 420.f, 480.f });
    shipCard3.setPosition({ 1330.f, 450.f });
    shipCard3.setFillColor(sf::Color(30, 30, 30));
    shipCard3.setOutlineThickness(6.f);

    sf::Sprite shipPreview1(texture);
    shipPreview1.setScale({ 500.f / texture.getSize().x, 300.f / texture.getSize().y });
    shipPreview1.setPosition({ 250.f, 480.f });

    sf::Sprite shipPreview2(shipTex2);
    shipPreview2.setScale({ 300.f / shipTex2.getSize().x, 300.f / shipTex2.getSize().y });
    shipPreview2.setPosition({ 850.f, 480.f });

    sf::Sprite shipPreview3(shipTex3);
    shipPreview3.setScale({ 300.f / shipTex3.getSize().x, 300.f / shipTex3.getSize().y });
    shipPreview3.setPosition({ 1390.f, 480.f });

    sf::Text shipLabel1(F, "BATTLESHIP 1\nRapture", 35);
    shipLabel1.setPosition({ 335.f, 840.f });
    shipLabel1.setFillColor(sf::Color::White);

    sf::Text shipLabel2(F, "BATTLESHIP 2\nMoon", 35);
    shipLabel2.setPosition({ 875.f, 840.f });
    shipLabel2.setFillColor(sf::Color::White);

    sf::Text shipLabel3(F, "BATTLESHIP 3\nZeta", 35);
    shipLabel3.setPosition({ 1415.f, 840.f });
    shipLabel3.setFillColor(sf::Color::White);

    // biome selection cards
    sf::Text biomeTitle(F, "SELECT YOUR BIOME", 70);
    biomeTitle.setStyle(sf::Text::Bold);
    biomeTitle.setFillColor(sf::Color::Cyan);
    biomeTitle.setPosition({ 620.f, 250.f });

    sf::RectangleShape biomeCard1({ 460.f, 420.f });
    biomeCard1.setPosition({ 200.f, 450.f });
    biomeCard1.setFillColor(sf::Color::Black);
    biomeCard1.setOutlineThickness(6.f);

    sf::RectangleShape biomeCard2({ 460.f, 420.f });
    biomeCard2.setPosition({ 770.f, 450.f });
    biomeCard2.setFillColor(sf::Color::Black);
    biomeCard2.setOutlineThickness(6.f);

    sf::RectangleShape biomeCard3({ 460.f, 420.f });
    biomeCard3.setPosition({ 1340.f, 450.f });
    biomeCard3.setFillColor(sf::Color::Black);
    biomeCard3.setOutlineThickness(6.f);

    sf::Sprite biomePreview1(back);
    biomePreview1.setScale({ 420.f / back.getSize().x, 280.f / back.getSize().y });
    biomePreview1.setPosition({ 220.f, 470.f });

    sf::Sprite biomePreview2(biomeTex2);
    biomePreview2.setScale({ 420.f / biomeTex2.getSize().x, 280.f / biomeTex2.getSize().y });
    biomePreview2.setPosition({ 790.f, 470.f });

    sf::Sprite biomePreview3(biomeTex3);
    biomePreview3.setScale({ 420.f / biomeTex3.getSize().x, 280.f / biomeTex3.getSize().y });
    biomePreview3.setPosition({ 1360.f, 470.f });

    sf::Text biomeLabel1(F, "BIOME 1\n (MOON)", 32);
    biomeLabel1.setPosition({ 295.f, 790.f });
    biomeLabel1.setFillColor(sf::Color::White);

    sf::Text biomeLabel2(F, "BIOME 2\n(MOTHERSHIP ZETA)", 32);
    biomeLabel2.setPosition({ 850.f, 790.f });
    biomeLabel2.setFillColor(sf::Color::White);

    sf::Text biomeLabel3(F, "BIOME 3\n(RAPTURE)", 32);
    biomeLabel3.setPosition({ 1500.f, 790.f });
    biomeLabel3.setFillColor(sf::Color::White);

    // tutorial ui panel
    sf::RectangleShape howToPanel({ 1300.f, 750.f });
    howToPanel.setPosition({ 350.f, 380.f });
    howToPanel.setFillColor(sf::Color(0, 0, 0, 210));
    howToPanel.setOutlineThickness(4.f);
    howToPanel.setOutlineColor(sf::Color::Cyan);

    sf::Text howToTitle(F, "HOW TO PLAY", 70);
    howToTitle.setStyle(sf::Text::Bold);
    howToTitle.setFillColor(sf::Color::Cyan);
    howToTitle.setPosition({ 750.f, 230.f });

    sf::Text howToBody(
        F,
        "1. Use ARROW KEYS (Left, Right, Up, Down) to move your battleship.\n\n"
        "2. Press SPACEBAR to fire lasers at incoming enemy ships.\n\n"
        "3. Destroy enemies to earn +20 points and beat the Highest Score.\n\n"
        "4. Use BATTLESHIP SELECTION or CHOOSE BIOME to customize visuals.\n\n"
        "5. Click SOUND: ON/OFF at the top-left anytime to mute/unmute.",
        36
    );
    howToBody.setFillColor(sf::Color::White);
    howToBody.setPosition({ 410.f, 440.f });

    sf::Clock clock;
    float speed = 400.f;


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
    int current_score = 0;

    sf::Text healthy(F1,"Health : 200 ",30);
    healthy.setFillColor(sf::Color(255,130,20));
    healthy.setPosition({50.f,320.f});
    healthy.setStyle(sf::Text::Bold | sf::Text::Italic);

    float enemy_bullet_speed = 500.f;
    float enemy_speed = 300.f;


    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (event->getIf<sf::Event::Resized>())
            {
                window.setView(gameView);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
            }

            // Fixed mouse input
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                   // Convert screen pixels accurately into 2000x1600 world coordinates
                sf::Vector2f clickPos = window.mapPixelToCoords(mousePressed->position);

                if (currentstate == Gamestate::MENU)
                {
                    if (startButton.getGlobalBounds().contains(clickPos))
                    {
                        currentstate = Gamestate::PLAYING;
                        Menu.stop();
                        Game.play();
                    }

                    if (Settings.getGlobalBounds().contains(clickPos))
                    {
                        customScreen = 0;
                        currentstate = Gamestate::SETTINGS;
                        Game.stop();

                    }

                    if (exitButton.getGlobalBounds().contains(clickPos))
                    {
                        window.close();
                    }
                }

                if (currentstate == Gamestate::PLAYING)
                {
                    if (backOption.getGlobalBounds().contains(clickPos))
                    {
                        currentstate = Gamestate::MENU;
                        Menu.play();
                        Game.stop();
                        enemybullets.clear();
                    }
                }

                if (currentstate == Gamestate::SETTINGS)
                {
                    if (backOption.getGlobalBounds().contains(clickPos))
                    {
                        currentstate = Gamestate::MENU;
                        Menu.play();
                    }
                }

                // Mute / Unmute universal toggle 
                if (muteBox.getGlobalBounds().contains(clickPos))
                {
                    isMuted = !isMuted;
                    if (isMuted)
                    {
                        Menu.setVolume(0.f);
                        Game.setVolume(0.f);
                        sound.setVolume(0.f);
                        muteButton.setString("SOUND: OFF");
                        muteButton.setFillColor(sf::Color::Red);
                        muteBox.setOutlineColor(sf::Color::Red);
                    }
                    else
                    {
                        Menu.setVolume(100.f);
                        Game.setVolume(50.f);
                        sound.setVolume(100.f);
                        muteButton.setString("SOUND: ON");
                        muteButton.setFillColor(sf::Color::Cyan);
                        muteBox.setOutlineColor(sf::Color::Cyan);
                    }
                }

                // main menu clicks for biome and tutorial 
                // if (prevState == Gamestate::MENU && currentstate == Gamestate::MENU)
                if(currentstate == Gamestate::MENU)
                {
                    if(Settings.getGlobalBounds().contains(clickPos))
                    {
                        customScreen = 0;
                        currentstate = Gamestate::SETTINGS;
                        Game.stop();

                    }
                    if (biomeBox.getGlobalBounds().contains(clickPos))
                    {
                        customScreen = 1;
                        currentstate = Gamestate::SETTINGS;
                        Game.stop();
                        gameMusicActive = false;
                    }
                    else if (howToPlayBox.getGlobalBounds().contains(clickPos))
                    {
                        customScreen = 2;
                        currentstate = Gamestate::SETTINGS;
                        Game.stop();
                        gameMusicActive = false;
                    }
                }
                // else if (prevState == Gamestate::SETTINGS && currentstate == Gamestate::SETTINGS)
                else if(currentstate == Gamestate::SETTINGS)
                {
                    // handle th 3 battleship choices
                    if (customScreen == 0)
                    {
                        if (shipCard1.getGlobalBounds().contains(clickPos))
                        {
                            selectedShip = 1;
                            battleship.setTexture(texture, true);
                            battleship.setScale({0.4,0.4});
                        }
                        else if (shipCard2.getGlobalBounds().contains(clickPos))
                        {
                            selectedShip = 2;
                            battleship.setTexture(shipTex2, true);
                            battleship.setScale({0.5f,0.5f});
                        }
                        else if (shipCard3.getGlobalBounds().contains(clickPos))
                        {
                            selectedShip = 3;
                            battleship.setTexture(shipTex3, true);
                            battleship.setScale({0.5f,0.5f});
                        }
                    }
                    // handle 3 biome choices
                    else if (customScreen == 1)
                    {
                        if (biomeCard1.getGlobalBounds().contains(clickPos))
                        {
                            selectedBiome = 1;
                            background.setTexture(back, true);
                        }
                        else if (biomeCard2.getGlobalBounds().contains(clickPos))
                        {
                            selectedBiome = 2;
                            background.setTexture(biomeTex2, true);
                        }
                        else if (biomeCard3.getGlobalBounds().contains(clickPos))
                        {
                            selectedBiome = 3;
                            background.setTexture(biomeTex3, true);
                        }
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

                   //enemy spawn after passage of time
            if (enemySpawnClock.getElapsedTime().asSeconds() > 1.8f && enemies.size() < 4)
            {
                Spawnnew(enemies, EnemyTexture, 2000.0f);
                enemySpawnClock.restart();
            }

           //player and enemy bullet firng
            for (auto bIt = bullets.begin(); bIt != bullets.end(); )
            {
                bIt->move({ 0.f, -600.f * dt });
                bool bulletRemoved = false;

                for (auto eIt = enemies.begin(); eIt != enemies.end(); )
                {
                    if (bIt->getGlobalBounds().findIntersection(eIt->sprite.getGlobalBounds()))
                    {
                        totalscore += 20;
                        scoreText.setString("Score: " + std::to_string(totalscore));

                        int highest = UpdatedHighestvalue(totalscore, highest_score);
                        high.setString("Highest Score: " + std::to_string(highest));

                        eIt = enemies.erase(eIt); // after fire enemy del
                        bIt = bullets.erase(bIt); 
                        bulletRemoved = true;
                        break;
                    }
                    else
                    {
                        ++eIt;
                    }
                }

                if (!bulletRemoved)
                {
                    if (bIt->getPosition().y < 0.f)
                    {
                        bIt = bullets.erase(bIt);
                    }
                    else
                    {
                        ++bIt;
                    }
                }
            }

            //enemy movement and seperate firing
            for (auto it = enemies.begin(); it != enemies.end(); )
            {
                it->sprite.move({ 0.f, it->speed * dt });
                //enenmy fire time
                if (it->shootClock.getElapsedTime().asSeconds() > it->shootinterval)
                {
                 // only fire when enemy is on screen
                    if (it->sprite.getPosition().y > 0.0f && it->sprite.getPosition().y < 1300.f)
                    {
                        sf::Sprite eBullet(enemybullettexture);
                        float eBulletX = it->sprite.getPosition().x + (it->sprite.getGlobalBounds().size.x / 2.f) - 4.f;
                        float eBulletY = it->sprite.getPosition().y + it->sprite.getGlobalBounds().size.y;

                        eBullet.setPosition({ eBulletX, eBulletY });
                        enemybullets.push_back(eBullet);
                    }
                    it->shootClock.restart();
                }

             
                if (it->sprite.getPosition().y > 1500.0f)
                {
                    it = enemies.erase(it);
                }
                else
                {
                    ++it;
                }
            }

             //enemy bulet movement and player intersection
            for (auto it = enemybullets.begin(); it != enemybullets.end(); )
            {
                it->move({ 0.f, enemy_bullet_speed* dt });

                if (it->getGlobalBounds().findIntersection(battleship.getGlobalBounds()))
                {
                    it = enemybullets.erase(it);
                    // enemybullets.clear();
                    // bullets.clear();
                    // enemies.clear();
                    health = health-10;
                    healthy.setString("Health : " + std::to_string(health));
                    if(health<=0)
                    {
                        currentstate = Gamestate::GAME_OVER;
                        Game.stop();
                        current_score=totalscore;
                        totalscore=0;
                        health=200;
                        enemy_bullet_speed=500.f;
                        enemy_speed = 300.f;
                        speed = 400.f;
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
            // // Enemy movement using delta time
            // enemy.sprite.move({ 0.f, enemy.speed * dt });

            // // Respawn if enemy moves below bottom of the screen
            // if (enemy.sprite.getPosition().y > 1400.0f)
            // {
            //     SpawnEnemies(enemy, 2000.0f);
            //     enemybullets.clear();
            // }
            // if (enemyshoot.getElapsedTime().asSeconds() > 1.2f)
            // {
            //     if (enemy.sprite.getPosition().y > 0.0f)
            //     {
            //         sf::Sprite eBullet(enemybullettexture);
            //         float eBulletX = enemy.sprite.getPosition().x + (enemy.sprite.getGlobalBounds().size.x / 2.f) - 4.f;
            //         float eBulletY = enemy.sprite.getPosition().y + enemy.sprite.getGlobalBounds().size.y;

            //         eBullet.setPosition({ eBulletX, eBulletY });
            //         enemybullets.push_back(eBullet);
            //     }
            //     enemyshoot.restart();
            // }
            // for (auto it = enemybullets.begin(); it != enemybullets.end(); )
            // {
            //     it->move({ 0.f, enemy_bullet_speed * dt });

            //     if (it->getGlobalBounds().findIntersection(battleship.getGlobalBounds()))
            //     {
            //         it = enemybullets.erase(it);
            //         // enemybullets.clear();
            //         // bullets.clear();
            //         // currentstate = Gamestate::MENU;
            //         // Menu.play();
            //         // Game.stop();
            //         // break;
                    // health = health-10;
                    // healthy.setString("Health : " + std::to_string(health));
                    // if(health<=0)
                    // {
                    //     currentstate = Gamestate::GAME_OVER;
                    //     Game.stop();
                    // }
            //     }
            //     else if (it->getPosition().y > 1600.f)
            //     {
            //         it = enemybullets.erase(it);
            //     }
            //     else
            //     {
            //         ++it;
            //     }
            // }

            if (totalscore % 100 == 0)
            {
                enemy_speed += 0.2;
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

            window.draw(biomeBox);
            window.draw(biomeButton);
            window.draw(howToPlayBox);
            window.draw(howToPlayButton);
            window.draw(muteBox);
            window.draw(muteButton);
        }
        else if (currentstate == Gamestate::PLAYING)
        {
            window.draw(background);
            window.draw(returnBox);
            window.draw(backOption);
            window.draw(battleship);
            for (const auto& en : enemies)
            {
                window.draw(en.sprite);
            }
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
            window.draw(muteBox);
            window.draw(muteButton);

        }
        else if (currentstate == Gamestate::SETTINGS)
        {
            window.draw(Image);
            // window.draw(box1);
            // window.draw(box2);
            // window.draw(box3);
            // window.draw(option1);
            // window.draw(returnBox);
            // window.draw(backOption);
            // window.draw(selectmenu);
             window.draw(returnBox);
            window.draw(backOption);
            window.draw(selectmenu);


            if (customScreen == 0)
            {
                //draw battleship selection screen
                window.draw(backs);
                window.draw(returnBox);
                window.draw(backOption);
                window.draw(selectmenu);

                if (selectedShip == 1)
                    shipCard1.setOutlineColor(sf::Color::Green);
                else
                    shipCard1.setOutlineColor(sf::Color::White);

                if (selectedShip == 2)
                    shipCard2.setOutlineColor(sf::Color::Green);
                else
                    shipCard2.setOutlineColor(sf::Color::White);

                if (selectedShip == 3)
                    shipCard3.setOutlineColor(sf::Color::Green);
                else
                    shipCard3.setOutlineColor(sf::Color::White);

                window.draw(shipCard1);
                window.draw(shipCard2);
                window.draw(shipCard3);
                window.draw(shipPreview1);
                window.draw(shipPreview2);
                window.draw(shipPreview3);
                window.draw(shipLabel1);
                window.draw(shipLabel2);
                window.draw(shipLabel3);
            }
            else if (customScreen == 1)
            {
                // draw buome selection screen
                window.draw(backs);
                window.draw(returnBox);
                window.draw(backOption);
                window.draw(biomeTitle);

                if (selectedBiome == 1)
                    biomeCard1.setOutlineColor(sf::Color::Green);
                else
                    biomeCard1.setOutlineColor(sf::Color::White);

                if (selectedBiome == 2)
                    biomeCard2.setOutlineColor(sf::Color::Green);
                else
                    biomeCard2.setOutlineColor(sf::Color::White);

                if (selectedBiome == 3)
                    biomeCard3.setOutlineColor(sf::Color::Green);
                else
                    biomeCard3.setOutlineColor(sf::Color::White);

                window.draw(biomeCard1);
                window.draw(biomeCard2);
                window.draw(biomeCard3);
                window.draw(biomePreview1);
                window.draw(biomePreview2);
                window.draw(biomePreview3);
                window.draw(biomeLabel1);
                window.draw(biomeLabel2);
                window.draw(biomeLabel3);
            }
            else if (customScreen == 2)
            {

                window.draw(backs);
                window.draw(returnBox);
                window.draw(backOption);
                window.draw(howToTitle);
                window.draw(howToPanel);
                window.draw(howToBody);
            }


            window.draw(muteBox);
            window.draw(muteButton);
        }
        else if(currentstate == Gamestate::GAME_OVER)
        {
            window.draw(End);
        }

        window.display();
    }

    return 0;
}