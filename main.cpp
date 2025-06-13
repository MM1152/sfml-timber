#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <SFML/Audio.hpp>

enum class Side {LEFT, RIGHT, NONE};
enum class GAMESTATUS { START , RESUME , RESTART};

sf::Vector2f windowSize;
bool checkOutofWindow(sf::Sprite* sprite);
void InitSprite(sf::Sprite* sprite, sf::Vector2f* dir , float* speed);
void spriteLeftStart(sf::Sprite* sprite, sf::Vector2f* dir);
void spriteRightStart(sf::Sprite* sprite, sf::Vector2f* dir);
void aroundMove(sf::Sprite* sprite , sf::Vector2f* dir);
void flipX(sf::Sprite* sprite, float dirX);
void updateBranch(Side* sprtie, int size);

float timer = 0.f;

bool onClick = false;
bool isPlaying = true;
bool oneTime = false;
bool onDownEnter = false;

int main()
{
    srand((int)time(0)); // 현재시간을 가져온다.

    const std::string resourcePath = "Resources/";
    const std::string graphicsPath = resourcePath + "graphics/";
    const std::string fontPath = resourcePath + "fonts/";
    const std::string soundPath = resourcePath + "sound/";

    sf::RenderWindow window(sf::VideoMode(1920 , 1080), "Timber!"); // 화면에 뛰울 윈도우의 사이즈 / 제목
    windowSize = { (sf::Vector2f)window.getSize() };

#pragma region Texture
    sf::Texture textureCloud;
    sf::Texture textureBee;
    sf::Texture textureTree;
    sf::Texture textureBackGround;
    sf::Texture texturePlayer;
    sf::Texture textureBranch;
    sf::Texture textureAxe;
    sf::Texture textureLog;

    textureCloud.loadFromFile(graphicsPath + "cloud.png");
    textureBee.loadFromFile(graphicsPath + "bee.png");
    textureTree.loadFromFile(graphicsPath + "tree.png");
    textureBackGround.loadFromFile(graphicsPath + "background.png");
    texturePlayer.loadFromFile(graphicsPath + "player.png");
    textureBranch.loadFromFile(graphicsPath + "branch.png");
    textureAxe.loadFromFile(graphicsPath + "axe.png");
    textureLog.loadFromFile(graphicsPath + "log.png");
#pragma endregion
#pragma region SoundBuffer
    sf::SoundBuffer bufferChop;
    sf::SoundBuffer bufferDeath;
    sf::SoundBuffer bufferOut_Of_Time;

    bufferChop.loadFromFile(soundPath + "chop.wav");
    bufferDeath.loadFromFile(soundPath + "death.wav");
    bufferOut_Of_Time.loadFromFile(soundPath + "out_of_time.wav");
#pragma endregion
#pragma region Font
    sf::Font font;
    font.loadFromFile(fontPath + "KOMIKAP_.ttf");
#pragma endregion
#pragma region Timer
    sf::RectangleShape timerBar;
    float timeBarWidth = 400;
    float timeBarHeight = 80;
    timerBar.setSize({ timeBarWidth , timeBarHeight });
    timerBar.setFillColor(sf::Color::Red);
    timerBar.setPosition(windowSize.x * 0.4, windowSize.y - 200.f);
    float timerSpeed = timeBarWidth / 5;
    float remaingTime = 5;
#pragma endregion
#pragma region Sprite
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
    sf::Sprite spriteLog[50];


    spriteTree.setPosition(1920 * 0.5, 0);
    spriteTree.setOrigin((int)textureTree.getSize().x / 2, 0.f);

    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        spriteBranch[i].setTexture(textureBranch);
        spriteBranch[i].setOrigin(-(textureTree.getSize().x * 0.5), 0.f);
        spriteBranch[i].setPosition(spriteTree.getPosition().x, i * 150.f);
    }

    sf::Sprite spritePlayer;
    spriteBackGround.setTexture(textureBackGround);
    spriteTree.setTexture(textureTree);
    spritePlayer.setTexture(texturePlayer);
    spriteAxe.setTexture(textureAxe);

    sf::Vector2u;
    spritePlayer.setOrigin(-(int)textureTree.getSize().x / 2, 0);
    spritePlayer.setPosition(spriteTree.getPosition().x, 700.f);
    Side palyerSide = Side::LEFT;

    spriteAxe.setOrigin(-((int)texturePlayer.getSize().x * 0.2), 0.f);
    spriteAxe.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y + 110.f);
    Side axeSide = Side::LEFT;


    const int NUM_LOGS = 50;
    sf::Vector2f gravity = { 0.f , 1000.f };
    float speedLog = 500.f;
    bool isActiveLog[NUM_LOGS] = { };
    sf::Vector2f dirLog[NUM_LOGS] = { };
    sf::Vector2f velocityLog[NUM_LOGS] = {};

    for (int i = 0; i < NUM_LOGS; i++) {
        spriteLog[i].setTexture(textureLog);
        spriteLog[i].setOrigin(textureLog.getSize().x / 2, textureLog.getSize().y);
        spriteLog[i].setPosition(spriteTree.getPosition().x, textureTree.getSize().y);
    }
    
    sf::Vector2f initLogPosition = spriteLog[0].getPosition();

    //가속도 운동

    Side sideLog;
#pragma endregion


#pragma region Text
    //UI
    sf::Text textScore;
    textScore.setFont(font);
    textScore.setString("SCORE : 0");
    textScore.setCharacterSize(100); // 세로 기준
    textScore.setFillColor(sf::Color::White);
    textScore.setPosition(20, 20);

    sf::Text textGameStatus;
    textGameStatus.setFont(font);
    textGameStatus.setCharacterSize(50);
    textGameStatus.setFillColor(sf::Color::Red);
    textGameStatus.setPosition(windowSize.x / 2, windowSize.y / 2);
    textGameStatus.setOrigin(textGameStatus.getLocalBounds().width / 2, textGameStatus.getLocalBounds().height / 2);
#pragma endregion
#pragma region Sound
    //Sound
    sf::Sound soundChop;
    sf::Sound soundDeath;
    sf::Sound soundOutOfTime;

    soundChop.setBuffer(bufferChop);
    soundDeath.setBuffer(bufferDeath);
    soundOutOfTime.setBuffer(bufferOut_Of_Time);
#pragma endregion    
    //SCORE 변수
    int score = 0;

    sf::Event event;
    sf::Clock clock;
    
    GAMESTATUS status = GAMESTATUS::START;
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
            
            if (!isPlaying) {
                if (event.KeyPressed == event.type && !onClick && (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)) {
                    soundChop.play();
                    int idx = 0;
                    for (int i = 0; i < NUM_LOGS; i++) {
                        if (!isActiveLog[i]) {
                            idx = i;
                            isActiveLog[i] = true;
                            break;
                        }
                    }
            
                    if (event.key.code == sf::Keyboard::Left) {
                        spriteLog[idx].setPosition(initLogPosition);
                        dirLog[idx] = { 1, -1 };
                        velocityLog[idx] = dirLog[idx] * speedLog;

                        palyerSide = Side::LEFT;
                        axeSide = Side::LEFT;
                        sideLog = Side::LEFT;
                        updateBranch(sideBranch, NUM_BRANCHES);
                    }
                    else if (event.key.code == sf::Keyboard::Right) {
                        spriteLog[idx].setPosition(initLogPosition);
                        dirLog[idx] = { -1, -1 };
                        velocityLog[idx] = dirLog[idx] * speedLog;

                        palyerSide = Side::RIGHT;
                        axeSide = Side::RIGHT;
                        sideLog = Side::RIGHT;
                        updateBranch(sideBranch, NUM_BRANCHES);
                    }
                    if (palyerSide == sideBranch[NUM_BRANCHES - 1]) {
                        score = 0;
                        printf("아야\n");
                        isPlaying = true;
                        soundDeath.play();
                        status = GAMESTATUS::RESTART;
                    }
                    else {
                        score += 10;
                    }
                    
                    
                    textScore.setString("SCORE: " + std::to_string(score));
                    onClick = true;
                }
                else if (event.KeyReleased == event.type) {
                    onClick = false;
                }

            }
            
            if (event.key.code == sf::Keyboard::Enter && !onDownEnter ) {
                if (!isPlaying) {
                    status = GAMESTATUS::RESUME;
                }
                
                isPlaying = !isPlaying;
                oneTime = false;
                if (status != GAMESTATUS::RESUME) {
                    remaingTime = 5;
                    timerBar.setSize({ timeBarWidth, timerBar.getSize().y });
                }
                if (status == GAMESTATUS::RESTART) {
                    updateBranch(sideBranch, NUM_BRANCHES);
                }
                onDownEnter = true;
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Enter) onDownEnter = false;
        }
        if (isPlaying) {
            if (status == GAMESTATUS::START) {
                textGameStatus.setString("Press Enter to start!");
            }
            else if (status == GAMESTATUS::RESUME) {
                textGameStatus.setString("Press Enter to resume!");
            }
            else if (status == GAMESTATUS::RESTART) {
                textGameStatus.setString("Press Enter to restart!");
            }
            textGameStatus.setOrigin(textGameStatus.getLocalBounds().width / 2, textGameStatus.getLocalBounds().height / 2);
        }



        //Update
        if (!isPlaying || !oneTime) {
            sf::Vector2f timerBarSize = timerBar.getSize();
            remaingTime -= deltaTime;
            timerBarSize.x -= timerSpeed * deltaTime;
            timerBar.setSize(timerBarSize);
            if (remaingTime <= 0) {
                isPlaying = true;
                status = GAMESTATUS::RESTART;
                score = 0;
                textScore.setString("SCORE: " + std::to_string(score));
                soundOutOfTime.play();
            }
        

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
            for (int i = 0; i < NUM_LOGS; i++) {
                if (isActiveLog[i]) {
                    sf::Vector2f pos = spriteLog[i].getPosition();
                    velocityLog[i] += gravity * deltaTime;
                    pos += velocityLog[i] * deltaTime;
                    spriteLog[i].setPosition(pos);

                    if (checkOutofWindow(&spriteLog[i])) isActiveLog[i] = false;
                }

            }
            if (isPlaying) oneTime = true;
        }



        //Draw
        window.clear();
       

        //World
        window.draw(spriteBackGround);
        
        
        window.draw(spriteTree);

        for (int i = 0; i < NUM_LOGS; i++) {
            if(isActiveLog[i]) window.draw(spriteLog[i]);
        }
        
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
        

        //UI
        window.draw(textScore);
        window.draw(timerBar);

        if(isPlaying) window.draw(textGameStatus);
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