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
        sprite.setScale({ 0.35f, 0.45f });
        shootinterval = 1.0f + static_cast<float>(std::rand() % 15) / 10.0f;
    };
};

void SpawnEnemies(ENemy& enemy, float windowWidth)
{
    sf::FloatRect bounds = enemy.sprite.getGlobalBounds();
    // float enemyWidth = bounds.size.x;
    // float enemyHeight = bounds.size.y;
    // float max = windowWidth - enemyWidth;
    // if (max <= 1.0f) max = 1.0f;
    // float random = static_cast<float>(std::rand() % static_cast<int>(max));
    float position[10][2]={{30.f,0},{230.f,0},{430.f,0},{630.f,0},{830,0},{1030,0},{1230,0},{1430,0},{1700,0},{1930,0}};
    int random = std::rand() % 10;
    enemy.sprite.setPosition({position[random][0], 0.f});
}

void HealthPoint(sf::Sprite &point, float windowWidth, float windowHeight)
{
   sf::FloatRect bound = point.getGlobalBounds();
   float max_width = windowWidth - 200- bound.size.x;
   float max_height = windowHeight -300-bound.size.y;
   float random = std::rand()%((int)max_width);
   float random1 = std::rand()%((int)max_height);
   if(random1<200)
   {
    random1 = random1 + 120;
   }
   if(random<200)
   {
    random = random + 120;
   }
   point.setScale({0.3,0.3});
   point.setPosition({random,random1});
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
    if (battleshipImage.loadFromFile("enemybattleship (1).png"))
    {
        battleshipImage.createMaskFromColor(sf::Color::White, 0);
    }

    sf::Texture texture;
    (void)texture.loadFromImage(battleshipImage);

    // Enemy ship (default loading)
    sf::Image obstacleImage;
    if (obstacleImage.loadFromFile("Enemy_battleship.png"))
    {
        obstacleImage.createMaskFromColor(sf::Color::White, 0);
    }

    //bullet image
    sf::Texture bul;
    if(!bul.loadFromFile("bullet_image (1).png"))
    {
        std::cout<<"Unable to load image";
    }

    sf::Texture EnemyTexture;
    (void)EnemyTexture.loadFromImage(obstacleImage);

    std::vector<ENemy> enemies;
    sf::Clock enemySpawnClock;
    
    // Background
    sf::Texture back;
    (void)back.loadFromFile("Back (2).jpeg");

    sf::Sprite background(back);
    background.setScale({ 1.4f, 1.3f });

    // Player Sprite setup (placed inside visible screen)
    sf::Sprite battleship(texture);
    battleship.setScale({0.4f,0.4f});
    battleship.setPosition({ 800.f, 1200.f });

    //Selection menu back image
    sf::Texture select;
    if(!select.loadFromFile("Select (1).jpeg"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Sprite Image(select);
    Image.setScale({ 1.3f,1.8f });

        //GameOver
    sf::Texture over;
    if(!over.loadFromFile("GameOver.jpeg"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Sprite End(over);
    End.setScale({1.3f,1.6f});

    // Background Audio
    sf::Music Menu;
    if (!Menu.openFromFile("back_music.mp3"))
    {
        std::cout << "Unable to load the menu background music\n";
    }
    else
    {
        Menu.setLooping(true);
        Menu.setVolume(200.f);
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
        Game.setVolume(100.f);
    }

    //Heart Beat Effect
    sf::Music Heart;
    if(!Heart.openFromFile("Heart_effect.mp3"))
    {
        std::cout<<"Unable to load the heart_beat effect";
    }
    else
    {
        Heart.setLooping(true);
        Heart.setVolume(400.f);
    }

    //Life gaining buffer sound
    sf::SoundBuffer Life;
    if(!Life.loadFromFile("life_voice.mp3"))
    {
        std::cout<<"Unable to load life voice";
    }
    sf::Sound life_voice(Life);
    life_voice.setVolume(400);

    //GameOver voice
    sf::Music voice;
    if(!voice.openFromFile("GameEnding.mp3"))
    {
        std::cout<<"Unable to load the game music";
    }
    else
    {
        voice.setLooping(false);
        voice.setVolume(150.f);
    }

    //bullets buffer voice
    sf::SoundBuffer bulletsound;
    if (!bulletsound.loadFromFile("laserGun.mp3"))
    {
        std::cout << "Unable to load buffer ";
    }
    sf::Sound sound(bulletsound);
    sound.setVolume(150);

    //crashing buffer sound of spaceships 
    sf::SoundBuffer Crashing;
    if(!Crashing.loadFromFile("Crash.mp3"))
    {
        std::cout<<"Unable to load buffer ";
    }
    sf::Sound crash(Crashing);
    crash.setVolume(150);

    //hitting bullets buffer sound
    sf::SoundBuffer hit;
    if(!hit.loadFromFile("hitting_effect.wav"))
    {
        std::cout<<"Unable to load buffer ";
    }
    sf::Sound hitting(hit);
    hitting.setVolume(150);

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

    sf::Text selectmenu(F, "SELECT YOUR FAVOURITE BATTLESHIP", 70);
    selectmenu.setStyle(sf::Text::Bold);
    selectmenu.setFillColor(sf::Color::White);
    selectmenu.setPosition({ 550.f,250 });

    //score
    int totalscore = 0;
    sf::Text score(F,"Score",25);
    score.setPosition({50.f,140.f});
    score.setFillColor(sf::Color::White);
    sf::Text scoreText(F1, "0", 50);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition({ 50.f, 170.f });
    scoreText.setStyle(sf::Text::Bold | sf::Text::Italic);

    //Level Box
    sf::RectangleShape levelbox({200,100});
    levelbox.setPosition({940,0});
    levelbox.setFillColor(sf::Color::Transparent);
    levelbox.setOutlineThickness({5});
    levelbox.setOutlineColor(sf::Color::Cyan);

    sf::Text lvl(F,"Level One \nGet ready",30);
    lvl.setPosition({980,10});
    lvl.setFillColor(sf::Color::White);


    sf::Texture backimage;
    if (!backimage.loadFromFile("Menu_back.jpeg"))
    {
        std::cout << "Failed to load background image 'menu_image.jpg'\n";
    }

    //images of background
    sf::Sprite backs(backimage);
    backs.setScale({ 1.3f, 1.8f });

    //Startbutton
    sf::RectangleShape rectangle({ 200,80 });
    rectangle.setPosition({ 870,890 });
    rectangle.setFillColor(sf::Color(25,18,10,210));
    rectangle.setOutlineThickness({4});
    rectangle.setOutlineColor(sf::Color(255,200,0));

    sf::Text startButton(F, "PLAY", 50);
    startButton.setPosition({ 915.f, 900.f });
    startButton.setFillColor(sf::Color::White);

    //Selection Menu
    sf::RectangleShape rectangle1({ 600,80 });
    rectangle1.setPosition({ 670,990 });
    rectangle1.setFillColor(sf::Color(25,18,10,210));
    rectangle1.setOutlineThickness({4});
    rectangle1.setOutlineColor(sf::Color(255,200,0));

    sf::Text Settings(F, "BATTLESHIP SELECTION", 50);
    Settings.setPosition({ 730.f,1000.f });
    Settings.setFillColor(sf::Color::White);

    //ExitButton
    sf::RectangleShape rectangle2({ 200,80 });
    rectangle2.setPosition({ 870,1090 });
    rectangle2.setFillColor(sf::Color(25,18,10,210));
    rectangle2.setOutlineThickness({4});
    rectangle2.setOutlineColor(sf::Color(255,200,0));

    sf::Text exitButton(F, "EXIT", 50);
    exitButton.setPosition({ 925.f, 1100.f });
    exitButton.setFillColor(sf::Color::White);

    //Return to Main Menu Option
    sf::Text backOption(F, "Return", 30);
    backOption.setFillColor(sf::Color::Black);
    backOption.setPosition({ 1810,40 });

    sf::RectangleShape returnBox({ 120,50 });
    returnBox.setPosition({ 1800,30 });
    returnBox.setFillColor(sf::Color::Red);
    returnBox.setOutlineThickness({4});
    returnBox.setOutlineColor(sf::Color::Black);

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
    if (!shipImg2.loadFromFile("battleship2 (1).png"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Texture shipTex2;
    (void)shipTex2.loadFromImage(shipImg2);

    sf::Image shipImg3;
    if (!shipImg3.loadFromFile("battleship3 (1).png"))
    {
        std::cout<<"Unable to load image";
    }
    
    sf::Texture shipTex3;
    (void)shipTex3.loadFromImage(shipImg3);

    //enemyships changing pics
    sf::Image enemyImg2;
    if(!enemyImg2.loadFromFile("enemy(2).png"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Texture enemy2(enemyImg2);

    sf::Image enemyImg3;
    if(!enemyImg3.loadFromFile("enemy(3).png"))
    {
        std::cout<<"Unable to load image";
    }
    sf::Texture enemy3(enemyImg3);

    //  Load 2 other biome background textures (itll go back to default if other files arent added)
    sf::Texture biomeTex2;
    if (!biomeTex2.loadFromFile("Back (1).jpeg"))
        (void)biomeTex2.loadFromFile("spaceback2.png");

    sf::Texture biomeTex3;
    if (!biomeTex3.loadFromFile("Back (3).jpeg"))
        (void)biomeTex3.loadFromFile("spaceback2.png");

    //  choose biome button 
    sf::RectangleShape biomeBox({ 430.f, 80.f });
    biomeBox.setPosition({ 765.f, 1090.f });
    biomeBox.setFillColor(sf::Color(25,18,10,210));
    biomeBox.setOutlineThickness({4});
    biomeBox.setOutlineColor(sf::Color(255,200,0));

    sf::Text biomeButton(F, "MAP SELECTION", 50);
    biomeButton.setPosition({ 805.f, 1100.f });
    biomeButton.setFillColor(sf::Color::White);

    // how to play button
    sf::RectangleShape howToPlayBox({ 390.f, 80.f });
    howToPlayBox.setPosition({ 785.f, 1190.f });
    howToPlayBox.setFillColor(sf::Color(25,18,10,210));
    howToPlayBox.setOutlineThickness({4});
    howToPlayBox.setOutlineColor(sf::Color(255,200,0));

    sf::Text howToPlayButton(F, "HOW TO PLAY", 50);
    howToPlayButton.setPosition({ 830.f, 1200.f });
    howToPlayButton.setFillColor(sf::Color::White);

    // exit button to the last
    rectangle2.setPosition({ 870.f, 1290.f });
    exitButton.setPosition({ 900.f, 1300.f });

    // sound mute unmute
    sf::RectangleShape muteBox({ 260.f, 65.f });
    muteBox.setPosition({ 50.f, 50.f });
    muteBox.setFillColor(sf::Color::Black);
    muteBox.setOutlineThickness(4.f);
    muteBox.setOutlineColor(sf::Color(255,200,0));

    sf::Text muteButton(F, "SOUND: ON", 32);
    muteButton.setPosition({ 78.f, 62.f });
    muteButton.setFillColor(sf::Color::White);

    // battleship selection cards 
    sf::RectangleShape shipCard1({ 420.f, 480.f });
    shipCard1.setPosition({ 250.f, 650.f });
    shipCard1.setFillColor(sf::Color(0, 0, 0, 210));
    shipCard1.setOutlineThickness(6.f);

    sf::RectangleShape shipCard2({ 420.f, 480.f });
    shipCard2.setPosition({ 790.f, 650.f });
    shipCard2.setFillColor(sf::Color(0, 0, 0, 210));
    shipCard2.setOutlineThickness(6.f);

    sf::RectangleShape shipCard3({ 420.f, 480.f });
    shipCard3.setPosition({ 1330.f, 650.f });
    shipCard3.setFillColor(sf::Color(0, 0, 0, 210));
    shipCard3.setOutlineThickness(6.f);

    sf::Sprite shipPreview1(texture);
    shipPreview1.setScale({ 300.f / texture.getSize().x, 300.f / texture.getSize().y });
    shipPreview1.setPosition({ 300.f, 680.f });

    sf::Sprite shipPreview2(shipTex2);
    shipPreview2.setScale({ 300.f / shipTex2.getSize().x, 300.f / shipTex2.getSize().y });
    shipPreview2.setPosition({ 850.f, 680.f });

    sf::Sprite shipPreview3(shipTex3);
    shipPreview3.setScale({ 300.f / shipTex3.getSize().x, 300.f / shipTex3.getSize().y });
    shipPreview3.setPosition({ 1390.f, 680.f });

    sf::Text shipLabel1(F, "BATTLESHIP 1\nRapture", 35);
    shipLabel1.setPosition({ 335.f, 1040.f });
    shipLabel1.setFillColor(sf::Color::White);

    sf::Text shipLabel2(F, "BATTLESHIP 2\nMoon", 35);
    shipLabel2.setPosition({ 875.f, 1040.f });
    shipLabel2.setFillColor(sf::Color::White);

    sf::Text shipLabel3(F, "BATTLESHIP 3\nZeta", 35);
    shipLabel3.setPosition({ 1415.f, 1040.f });
    shipLabel3.setFillColor(sf::Color::White);

    // biome selection cards
    sf::Text biomeTitle(F, "SELECT YOUR BACKGROUND MAP", 70);
    biomeTitle.setStyle(sf::Text::Bold);
    biomeTitle.setFillColor(sf::Color::White);
    biomeTitle.setPosition({ 630.f, 250.f });

    sf::RectangleShape biomeCard1({ 460.f, 420.f });
    biomeCard1.setPosition({ 200.f, 650.f });
    biomeCard1.setFillColor(sf::Color(0, 0, 0, 210));
    biomeCard1.setOutlineThickness(6.f);

    sf::RectangleShape biomeCard2({ 460.f, 420.f });
    biomeCard2.setPosition({ 770.f, 650.f });
    biomeCard2.setFillColor(sf::Color(0, 0, 0, 210));
    biomeCard2.setOutlineThickness(6.f);

    sf::RectangleShape biomeCard3({ 460.f, 420.f });
    biomeCard3.setPosition({ 1340.f, 650.f });
    biomeCard3.setFillColor(sf::Color(0, 0, 0, 210));
    biomeCard3.setOutlineThickness(6.f);

    sf::Sprite biomePreview1(back);
    biomePreview1.setScale({ 420.f / back.getSize().x, 280.f / back.getSize().y });
    biomePreview1.setPosition({ 220.f, 670.f });

    sf::Sprite biomePreview2(biomeTex2);
    biomePreview2.setScale({ 420.f / biomeTex2.getSize().x, 280.f / biomeTex2.getSize().y });
    biomePreview2.setPosition({ 790.f, 670.f });

    sf::Sprite biomePreview3(biomeTex3);
    biomePreview3.setScale({ 420.f / biomeTex3.getSize().x, 280.f / biomeTex3.getSize().y });
    biomePreview3.setPosition({ 1360.f, 670.f });

    sf::Text biomeLabel1(F, "BIOME 1\n (MOON)", 32);
    biomeLabel1.setPosition({ 295.f, 990.f });
    biomeLabel1.setFillColor(sf::Color::White);

    sf::Text biomeLabel2(F, "BIOME 2\n(MOTHERSHIP ZETA)", 32);
    biomeLabel2.setPosition({ 850.f, 990.f });
    biomeLabel2.setFillColor(sf::Color::White);

    sf::Text biomeLabel3(F, "BIOME 3\n(RAPTURE)", 32);
    biomeLabel3.setPosition({ 1500.f, 990.f });
    biomeLabel3.setFillColor(sf::Color::White);

    // tutorial ui panel
    sf::RectangleShape howToPanel({ 1300.f, 750.f });
    howToPanel.setPosition({ 350.f, 380.f });
    howToPanel.setFillColor(sf::Color(0, 0, 0, 210));
    howToPanel.setOutlineThickness(4.f);
    howToPanel.setOutlineColor(sf::Color::Cyan);

    sf::Text howToTitle(F, "HOW TO PLAY", 70);
    howToTitle.setStyle(sf::Text::Bold);
    howToTitle.setFillColor(sf::Color::White);
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

    //Reverse Bullet resources
    sf::Image bullet;
    if(!bullet.loadFromFile("reverse_bullet_image (1).png"))
    {
        std::cout<<"Unable to load image";
    }
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
    sf::Text h(F,"Highest Score ",25);
    h.setPosition({50,260});
    h.setFillColor(sf::Color::White);
    sf::Text high(F1,std::to_string(highest_score), 40);
    high.setFillColor(sf::Color::Magenta);
    high.setPosition({ 50.f,300.f });
    high.setStyle(sf::Text::Bold | sf::Text::Italic);


    //health part
    int health = 200;
    int current_score = 0;
    
    sf::Text life(F,"Health",25);
    life.setPosition({50.f,380.f});
    life.setFillColor(sf::Color::White);
    sf::Text healthy(F1,"200",50);
    healthy.setFillColor(sf::Color(255,130,20));
    healthy.setPosition({50.f,420.f});
    healthy.setStyle(sf::Text::Bold | sf::Text::Italic);

    //health point in game
    sf::Texture HP;
    if(!HP.loadFromFile("health_button.png"))
    {
        std::cout<<"Unable to load image of button";
    }
    sf::Sprite hp(HP);
    // hp.setScale({0.5,0.5});

    //displays health point if hpAvailable is true only
    bool hpAvailable = false;
    sf::Clock hpSpawnClock;
    sf::Clock hpLifetime;

    //Time for life button to exist on the screen
    float hplife = 10.f;

    float enemy_bullet_speed = 500.f;
    float enemy_speed = 300.f;

    bool heart_eff = false;


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
                    //Heart beat effect
                    if(health<50)
                    {
                        Heart.play();
                        Game.stop();
                    }
                    else
                    {
                        Heart.stop();
                        Game.play();
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
                        life_voice.setVolume(0.f);
                        Heart.setVolume(0.f);
                        crash.setVolume(0.f);
                        hitting.setVolume(0.f);
                        muteButton.setString("SOUND: OFF");
                        muteButton.setFillColor(sf::Color::Red);
                        muteBox.setOutlineColor(sf::Color::Red);
                    }
                    else
                    {
                        Menu.setVolume(100.f);
                        Game.setVolume(50.f);
                        sound.setVolume(100.f);
                        Heart.setVolume(400.f);
                        hitting.setVolume(150.f);
                        crash.setVolume(150.f);
                        life_voice.setVolume(400.f);
                        muteButton.setString("SOUND: ON");
                        muteButton.setFillColor(sf::Color::White);
                        muteBox.setOutlineColor(sf::Color(255,200,0));
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

                //Game Over state
                if(currentstate == Gamestate::GAME_OVER)
                {
                    voice.play();
                    if (backOption.getGlobalBounds().contains(clickPos))
                    {
                        voice.stop();
                        currentstate = Gamestate::MENU;
                        Menu.play();
                    }
                    if (muteBox.getGlobalBounds().contains(clickPos))
                    {
                        voice.stop();   
                         isMuted = !isMuted;
                        if (isMuted)
                       {
                        Menu.setVolume(0.f);
                        Game.setVolume(0.f);
                        sound.setVolume(0.f);
                        voice.setVolume(0.f);
                        Heart.setVolume(0.f);
                        muteButton.setString("SOUND: OFF");
                        muteButton.setFillColor(sf::Color::Red);
                        muteBox.setOutlineColor(sf::Color::Red);
                       }
                       else
                       {
                        Menu.setVolume(200.f);
                        Game.setVolume(100.f);
                        sound.setVolume(150.f);
                        voice.setVolume(150.f);
                        Heart.setVolume(400.f);
                        muteButton.setString("SOUND: ON");
                        muteButton.setFillColor(sf::Color::Cyan);
                        muteBox.setOutlineColor(sf::Color::Cyan);
                       }
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
                            battleship.setScale({0.5,0.5});
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


        bool label=false;
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
                if (battleship.getPosition().y > 0.f)
                {
                    battleship.move({ 0.f, -speed * dt });
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
            {
                if (battleship.getPosition().y < 1600.f - battleship.getGlobalBounds().size.y)
                {
                    battleship.move({ 0.f, speed * dt });
                }
            }

            // Firing
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                if (shoot.getElapsedTime().asSeconds() > 0.15f)
                {
                    sf::Sprite newBullet(bul);
                    newBullet.setScale({0.2,0.3});
                    float bulletx = battleship.getPosition().x + (battleship.getGlobalBounds().size.x / 2.f) - 4.f;
                    float bullety = battleship.getPosition().y;

                    newBullet.setPosition({ bulletx, bullety});
                    bullets.push_back(newBullet);
                    sound.play();


                    shoot.restart();
                }
            }

                   //enemy spawn after passage of time
            if ((enemySpawnClock.getElapsedTime().asSeconds() > 1.8f && enemies.size() < 4)&&(totalscore<=400))
            {
                Spawnnew(enemies, EnemyTexture, 2000.0f);
                enemySpawnClock.restart();
            }
            else if ((enemySpawnClock.getElapsedTime().asSeconds() > 1.8f && enemies.size() < 4)&&(totalscore<=800))
            {
                Spawnnew(enemies, enemy2, 2000.0f);
                enemySpawnClock.restart();
            }
            else if ((enemySpawnClock.getElapsedTime().asSeconds() > 1.8f && enemies.size() < 4)&&(totalscore>800))
            {
                Spawnnew(enemies, enemy3, 2000.0f);

                if(totalscore%100==0)
                {
                    Spawnnew(enemies, enemy2, 2000.0f);
                    enemySpawnClock.restart();
                }
                else if(totalscore%50==0)
                {
                    Spawnnew(enemies, EnemyTexture, 2000.0f);
                    enemySpawnClock.restart();
                }
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
                        totalscore += 10;
                        scoreText.setString(std::to_string(totalscore));

                        int highest = UpdatedHighestvalue(totalscore, highest_score);
                        high.setString(std::to_string(highest));

                        eIt = enemies.erase(eIt); // after fire enemy del
                        bIt = bullets.erase(bIt); 
                        bulletRemoved = true;
                        hitting.play();
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
                        sf::Sprite eBullet(bullettexture);
                        eBullet.setScale({0.2,0.3});
                        float eBulletX = it->sprite.getPosition().x + (it->sprite.getGlobalBounds().size.x / 2.f) - 4.f;
                        float eBulletY = it->sprite.getPosition().y + it->sprite.getGlobalBounds().size.y;

                        eBullet.setPosition({ eBulletX, eBulletY});
                        enemybullets.push_back(eBullet);
                    }
                    it->shootClock.restart();
                }

                if (it->sprite.getGlobalBounds().findIntersection(battleship.getGlobalBounds()))
                {
                    health = health-10;
                    healthy.setString(std::to_string(health));
                    it = enemies.erase(it);
                    crash.play();
                }
             
                else if (it->sprite.getPosition().y > 1600.0f)
                {
                    it = enemies.erase(it);
                    health = health - 10;
                    healthy.setString(std::to_string(health));
                    crash.play();
                }
                else
                {
                    ++it;
                }
            }

            // Spawn health point every 15 seconds
            if (!hpAvailable && hpSpawnClock.getElapsedTime().asSeconds() > 10)
            {
                HealthPoint(hp,1800.f,1400.f);
                hpAvailable = true;
                hpLifetime.restart();
                hpSpawnClock.restart();
            }
            // Collect health point
            if (hpAvailable && battleship.getGlobalBounds().findIntersection(hp.getGlobalBounds()))
            {
                health += 50;
                if (health > 200)
                health = 200;
                healthy.setString(std::to_string(health));
                life_voice.play();
                hpAvailable = false;
                hpSpawnClock.restart();
                hpLifetime.restart();
            }

            //removal of hp button after 5 sec
            if(hpAvailable && hpLifetime.getElapsedTime().asSeconds() > hplife)
            {
                hpAvailable = false;
                hpLifetime.restart();
                hpSpawnClock.restart();
            }

            //Heart beat effect
            if((health<50)&&(!heart_eff))
            {
                Heart.play();
                Game.stop();
                heart_eff = true;
            }
            else if(health>50)
            {
                Heart.stop();
                Game.play();
                heart_eff = false;
            }

             //enemy bulet movement and player intersection
            for (auto it = enemybullets.begin(); it != enemybullets.end(); )
            {
                it->move({ 0.f, enemy_bullet_speed* dt });
                if(health<=0)
                {
                    currentstate = Gamestate::GAME_OVER;
                    Game.stop();
                    voice.play();
                    current_score = totalscore;
                    totalscore=0;
                    health=200;
                    enemy_bullet_speed=500.f;
                    enemy_speed = 300.f;
                    speed = 400.f;
                }

                if (it->getGlobalBounds().findIntersection(battleship.getGlobalBounds()))
                {
                    it = enemybullets.erase(it);
                    // enemybullets.clear();
                    // bullets.clear();
                    // enemies.clear();
                    health = health-10;
                    healthy.setString(std::to_string(health));
                    hitting.play();
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
            if(totalscore==500)
            {
                lvl.setString("Level Two\n Medium");
            }
            if(totalscore==1000)
            {
                lvl.setString("Boss Level\nDifficult");
            }

            if (totalscore % 200 == 0)
            {
                enemy_speed += 0.2;
                enemy_bullet_speed += 0.2;
            }

            if (totalscore % 300 == 0)
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
            if(hpAvailable)
            {
                window.draw(hp);
            }
            window.draw(scoreText);
            window.draw(high);
            window.draw(healthy);
            window.draw(muteBox);
            window.draw(muteButton);
            window.draw(h);
            window.draw(score);
            window.draw(life);
            window.draw(levelbox);
            window.draw(lvl);

        }
        else if (currentstate == Gamestate::SETTINGS)
        {
            window.draw(Image);
            window.draw(returnBox);
            window.draw(backOption);


            if (customScreen == 0)
            {
                //draw battleship selection screen
                // window.draw(backs);
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
                // draw biome selection screen
                // window.draw(backs);
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

                // window.draw(backs);
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
            window.draw(returnBox);
            window.draw(backOption);
            window.draw(muteBox);
            window.draw(muteButton);
        }

        window.display();
    }

    return 0;
}