#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define MAX_HEIGHT(value) value + 30.f
#define MIN_HEIGHT(value) value - 30.f

sf::Vector2f windowSize;
bool checkOutofWindow(sf::Sprite* sprite);
void InitSprite(sf::Sprite* sprite, sf::Vector2f* dir , float* speed);
void spriteLeftStart(sf::Sprite* sprite, sf::Vector2f* dir);
void spriteRightStart(sf::Sprite* sprite, sf::Vector2f* dir);
void parabolaMove(sf::Sprite* sprite , sf::Vector2f* dir , float initHeight);

int main()
{
    srand((int)time(0)); // 현재시간을 가져온다.

    const std::string resourcePath = "Resources/";
    const std::string graphicsPath = resourcePath + "graphics/";

    sf::RenderWindow window(sf::VideoMode(1920 , 1080), "Timber!"); // 화면에 뛰울 윈도우의 사이즈 / 제목
    windowSize = { (sf::Vector2f)window.getSize() };
    sf::Texture textureCloud;
    sf::Texture textureBee;
    sf::Texture textureTree;
    sf::Texture textureBackGround;

    textureCloud.loadFromFile(graphicsPath + "cloud.png");
    textureBee.loadFromFile(graphicsPath + "bee.png");
    textureTree.loadFromFile(graphicsPath + "tree.png");
    textureBackGround.loadFromFile(graphicsPath + "background.png");

    sf::Sprite spriteBackGround;
    sf::Sprite spriteCloud1;
    sf::Sprite spriteCloud2;
    sf::Sprite spriteCloud3;
    sf::Sprite spriteBee;
    sf::Sprite spriteBee1;
    sf::Sprite spriteTree;
    
    spriteBackGround.setTexture(textureBackGround);
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteTree.setTexture(textureTree);
    spriteBee.setTexture(textureBee);
    spriteBee1.setTexture(textureBee);

    spriteTree.setPosition(1920 * 0.5 , 0);
    spriteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.f);
    
    spriteBee.setPosition(textureBee.getSize().x / 2, 1080 * 0.7 - textureBee.getSize().y / 2);
    spriteBee.setOrigin(textureBee.getSize().x / 2, textureBee.getSize().y / 2);
    spriteBee1.setPosition(textureBee.getSize().x / 2, 1080 * 0.7 - textureBee.getSize().y / 2);
    spriteBee1.setOrigin(textureBee.getSize().x / 2, textureBee.getSize().y / 2);

    sf::Vector2f beeDir = { 1.f , 0.f };
    float beeSpeed = 500.f;
    InitSprite(&spriteBee , &beeDir , &beeSpeed);
    
    sf::Vector2f bee1Dir = { 0.f , 0.f };
    float bee1Speed = 500.f;
    float bee1SpeedY = 30.f;
    float bee1InitHeight = 0.f;
    
    InitSprite(&spriteBee1, &bee1Dir, &bee1Speed);
    parabolaMove(&spriteBee1, &bee1Dir , bee1InitHeight);

    bee1InitHeight = spriteBee1.getPosition().y;
    spriteCloud1.setPosition(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2);
    sf::Vector2f cloud1Dir = { 0,  0 };
    float cloud1speed = 500.f;
    InitSprite(&spriteCloud1, &cloud1Dir , &cloud1speed);

    spriteCloud2.setPosition(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2 * 3);
    sf::Vector2f cloud2Dir = { 0,  0 };
    float cloud2speed = 500.f;
    InitSprite(&spriteCloud2, &cloud2Dir, &cloud2speed);

    spriteCloud3.setPosition(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2 * 5);
    sf::Vector2f cloud3Dir = { 0,  0 };
    float cloud3speed = 500.f;
    InitSprite(&spriteCloud3, &cloud3Dir, &cloud3speed);

    spriteCloud1.setOrigin(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2);
    spriteCloud2.setOrigin(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2);
    spriteCloud3.setOrigin(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2);

    sf::Event event;
    sf::Clock clock;
   

    
    while (window.isOpen())
    {   
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();

        // 메세지 루프 -> 큐에담긴 메세지가 존재하면 while 문은 true
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Update
        spriteBee.setPosition(spriteBee.getPosition() + beeDir * beeSpeed * deltaTime);
        spriteBee1.setPosition(spriteBee1.getPosition().x + bee1Dir.x * beeSpeed * deltaTime , spriteBee1.getPosition().y + bee1Dir.y * bee1SpeedY * deltaTime);
        parabolaMove(&spriteBee1, &bee1Dir , bee1InitHeight);
        

        spriteCloud1.setPosition(spriteCloud1.getPosition() + cloud1Dir * cloud1speed * deltaTime);
        spriteCloud3.setPosition(spriteCloud3.getPosition() + cloud3Dir * cloud3speed * deltaTime);
        spriteCloud2.setPosition(spriteCloud2.getPosition() + cloud2Dir * cloud2speed * deltaTime);

        if (checkOutofWindow(&spriteBee)) 
        {
            InitSprite(&spriteBee , &beeDir, &beeSpeed);
        }
        if (checkOutofWindow(&spriteCloud1))
        {
            InitSprite(&spriteCloud1, &cloud1Dir, &cloud1speed);
        }
        if (checkOutofWindow(&spriteCloud2))
        {
            InitSprite(&spriteCloud2, &cloud2Dir , &cloud2speed);
        }
        if (checkOutofWindow(&spriteCloud3))
        {
            InitSprite(&spriteCloud3, &cloud3Dir , &cloud3speed);
        }
        if (checkOutofWindow(&spriteBee1)) {
            InitSprite(&spriteBee1, &bee1Dir, &bee1Speed);
        }

        

        //Draw
        window.clear();
        window.draw(spriteBackGround);
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        window.draw(spriteTree);
        window.draw(spriteBee1);
       // window.draw(spriteBee);
        window.display();
    }

    return 0;
}


bool checkOutofWindow(sf::Sprite* sprite) {
    if (sprite->getPosition().x >= windowSize.x + 100.f || sprite->getPosition().x < 0 - 100.f) {
        return true;
    }
    return false;
}
void InitSprite(sf::Sprite* sprite, sf::Vector2f* dir , float* speed) {
    float random = (float)rand() / RAND_MAX;
    if (random > 0.5f)
    {
        spriteLeftStart(sprite, dir);
        sprite->setScale({ -1.f , 1.f });
    }
    else
    {
        spriteRightStart(sprite, dir);
        sprite->setScale({ 1.f , 1.f });
    }
    *speed = (float)(rand() % 500 + 100);
}
void spriteLeftStart(sf::Sprite* sprite, sf::Vector2f* dir) {
    sprite->setPosition(-50, sprite->getPosition().y);
    *dir = { 1.f , 0.f };
}

void spriteRightStart(sf::Sprite* sprite, sf::Vector2f* dir) {
    sprite->setPosition(windowSize.x + 50, sprite->getPosition().y);
    *dir = { -1.f , 0.f };
}

void parabolaMove(sf::Sprite* sprite , sf::Vector2f* dir , float initHeight) {
    if (dir->y == 0) 
    {
        *dir = { dir->x , 1.f };
        return;
    }
    printf("pos : %f\n", sprite->getPosition().y);
    printf("maxHeight : %f\n", MAX_HEIGHT(initHeight));
    if (sprite->getPosition().y >= MAX_HEIGHT(initHeight))
    {
        *dir = { dir->x , -1.f };
    }
    if (sprite->getPosition().y <= MIN_HEIGHT(initHeight))
    {
        *dir = { dir->x , 1.f };
    }
}