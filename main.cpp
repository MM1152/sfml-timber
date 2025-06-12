#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>

enum class Side {LEFT, RIGHT, NONE};


sf::Vector2f windowSize;
bool checkOutofWindow(sf::Sprite* sprite);
void InitSprite(sf::Sprite* sprite, sf::Vector2f* dir , float* speed);
void spriteLeftStart(sf::Sprite* sprite, sf::Vector2f* dir);
void spriteRightStart(sf::Sprite* sprite, sf::Vector2f* dir);
void aroundMove(sf::Sprite* sprite , sf::Vector2f* dir);
void flipX(sf::Sprite* sprite, float dirX);
void updateBranch(Side* sprtie, int size);


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
    sf::Texture texturePlayer;
    sf::Texture textureBranch;
    sf::Texture textureAxe;

    textureCloud.loadFromFile(graphicsPath + "cloud.png");
    textureBee.loadFromFile(graphicsPath + "bee.png");
    textureTree.loadFromFile(graphicsPath + "tree.png");
    textureBackGround.loadFromFile(graphicsPath + "background.png");
    texturePlayer.loadFromFile(graphicsPath + "player.png");
    textureBranch.loadFromFile(graphicsPath + "branch.png");
    textureAxe.loadFromFile(graphicsPath + "axe.png");

    sf::Sprite moveAbleSprites[5];
    sf::Vector2f dir[5];
    float speed[5];
    for (int i = 0; i < sizeof(moveAbleSprites) / sizeof(moveAbleSprites[0]); i++) {
        if (i < 3) {
            moveAbleSprites[i].setTexture(textureCloud);
            moveAbleSprites[i].setPosition(textureCloud.getSize().x / 2, textureCloud.getSize().y * i);
            InitSprite(&moveAbleSprites[i], &dir[i], &speed[i]);
        }
        else {
            moveAbleSprites[i].setTexture(textureBee);
            moveAbleSprites[i].setPosition({ 400.f , 800.f });
            InitSprite(&moveAbleSprites[i], &dir[i], &speed[i]);
        }
        //moveAbleSprites[i].setOrigin(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2);
     }
    const int NUM_BRANCHES = 6;

    sf::Sprite spriteBackGround;
    sf::Sprite spriteBranch[NUM_BRANCHES];
    sf::Sprite spriteTree;
    sf::Sprite spriteAxe;

    spriteTree.setPosition(1920 * 0.5, 0);
    spriteTree.setOrigin(textureTree.getSize().x / 2, 0.f);
    
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        spriteBranch[i].setTexture(textureBranch);
        spriteBranch[i].setOrigin(-(textureTree.getSize().x * 0.5)  , 0.f);
        spriteBranch[i].setPosition(spriteTree.getPosition().x , i * 150.f);
    }


    sf::Sprite spritePlayer;
    spriteBackGround.setTexture(textureBackGround);
    spriteTree.setTexture(textureTree);
    spritePlayer.setTexture(texturePlayer);
    spriteAxe.setTexture(textureAxe);

    sf::Vector2u;
    spritePlayer.setOrigin(-(int)textureTree.getSize().x/2, 0);
    spritePlayer.setPosition(spriteTree.getPosition().x, 700.f);
    Side palyerSide = Side::LEFT;

    spriteAxe.setOrigin(-((int)texturePlayer.getSize().x * 0.2), 0.f);
    spriteAxe.setPosition(spritePlayer.getPosition().x , spritePlayer.getPosition().y + 110.f);
    Side axeSide = Side::LEFT;

    sf::Event event;
    sf::Clock clock;
    float timer = 0.f;
    bool onClick = false;
    bool isDie = false;

    Side sideBranch[NUM_BRANCHES] = { Side::LEFT,Side::RIGHT,Side::NONE,Side::RIGHT,Side::LEFT,Side::NONE};
    while (window.isOpen())
    {   
        sf::Time time = clock.restart();
        float deltaTime = time.asSeconds();
        timer += deltaTime;
        // 메세지 루프 -> 큐에담긴 메세지가 존재하면 while 문은 true
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.key.code == sf::Keyboard::Enter && isDie) {
                isDie = false;
            }
            if (!isDie) {
                if (event.KeyPressed == event.type && !onClick && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)) {
                    if (event.key.code == sf::Keyboard::Left) {
                        palyerSide = Side::LEFT;
                        axeSide = Side::LEFT;
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        palyerSide = Side::RIGHT;
                        axeSide = Side::RIGHT;
                    }
                    updateBranch(sideBranch, NUM_BRANCHES);
                    if (palyerSide == sideBranch[NUM_BRANCHES - 1]) {
                        printf("아야\n");
                        isDie = true;
                    }
                    onClick = true;
                }
                else if (event.KeyReleased == event.type) {
                    onClick = false;
                }
            }

          
        }

        //Update
        if (!isDie) {
            for (int i = 0; i < sizeof(moveAbleSprites) / sizeof(moveAbleSprites[0]); i++) {
                sf::Vector2f pos = moveAbleSprites[i].getPosition() + dir[i] * speed[i] * deltaTime;
                moveAbleSprites[i].setPosition(pos);
                if (checkOutofWindow(&moveAbleSprites[i]) && i != sizeof(moveAbleSprites) / sizeof(moveAbleSprites[0]) - 1) {
                    InitSprite(&moveAbleSprites[i], &dir[i], &speed[i]);
                }

                if (i == sizeof(moveAbleSprites) / sizeof(moveAbleSprites[0]) - 1) {
                    if (timer >= 2) {
                        aroundMove(&moveAbleSprites[i], &dir[i]);
                        timer = 0;
                    }
                    else if (checkOutofWindow(&moveAbleSprites[i])) {
                        dir[i].x = -(dir[i].x);
                        dir[i].y = -(dir[i].y);
                        flipX(&moveAbleSprites[i], dir[i].x);
                    }
                }
            }

            for (int i = 0; i < NUM_BRANCHES; i++) {
                switch (sideBranch[i]) {
                case Side::LEFT:
                        spriteBranch[i].setScale({ -1,1 });
                        break;
                    case Side::RIGHT:
                        spriteBranch[i].setScale({ 1,1 });
                        break;
                    }
                }

                switch (palyerSide) {
                case Side::LEFT:
                    spritePlayer.setScale({ -1 , 1 });
                    break;
                case Side::RIGHT:
                    spritePlayer.setScale({ 1 , 1 });
                    break;
                }
                switch (axeSide) {
                case Side::LEFT:
                    spriteAxe.setScale({ -1 , 1 });
                    break;
                case Side::RIGHT:
                    spriteAxe.setScale({ 1 , 1 });
                    break;
                }
        }



        //Draw
        window.clear();
        window.draw(spriteBackGround);
        window.draw(spriteTree);
        for (int i = 0; i < NUM_BRANCHES; i++) {
            if(sideBranch[i] != Side::NONE) window.draw(spriteBranch[i]);
            
        }
        window.draw(spritePlayer);
        if (onClick) {
            window.draw(spriteAxe);
        }
        
        for (sf::Sprite sp : moveAbleSprites) {
            window.draw(sp);
        }
        
       // window.draw(spriteBee);
        window.display();
    }

    return 0;
}


bool checkOutofWindow(sf::Sprite* sprite) {
    if (sprite->getPosition().x >= windowSize.x + 100.f || sprite->getPosition().x < 0 - 100.f) {
        return true;
    }
    if (sprite->getPosition().y >= windowSize.y || sprite->getPosition().y < 0) {
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

void flipX(sf::Sprite* sprite, float dirX) {
    if (dirX > 0) {
        sprite->setScale({-1 , 1});
    }
    else {
        sprite->setScale({ 1 , 1 });
    }
}

void updateBranch(Side* side, int size)
{
    for (int i = size - 1; i >= 0; i--) {
        side[i] = side[i - 1];
    }

    int r = rand() % 3;
    if (r == 0) side[0] = Side::LEFT;
    else if (r == 1) side[0] = Side::RIGHT;
    else if (r == 2) side[0] = Side::NONE;
}

void aroundMove(sf::Sprite* sprite , sf::Vector2f* dir) {
    if (checkOutofWindow(sprite)) return;

    printf("%f\n", rand() / (RAND_MAX * 0.5f) - 1);
    dir->x = rand() / (RAND_MAX * 0.5f) - 1;
    dir->y = rand() / (RAND_MAX * 0.5f) - 1;
    
    flipX(sprite, dir->x);
}