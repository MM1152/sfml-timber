
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#define NUM_BRANCHS 6
#define NUM_LOGS 10
#define TIMTER_SIZE_WIDTH 500
#define TIMER_SIZE_HEIGHT 100
sf::Vector2f windowSize;
std::string path = "Resources/";
std::string graphicsPath = path + "graphics/";
std::string fontPath = path + "fonts/";


enum class SIDE { LEFT, RIGHT, NONE };

void spriteInit(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f origin = { 0 , 0 }, sf::Vector2f position = { 0 , 0 });
void moveObject(sf::Sprite& sprite, sf::Vector2f dir, float speed);
bool checkOutOfWindow(sf::Sprite& sprite);
void resetObject(sf::Sprite& sprite, sf::Vector2f& dir, float& speed);
void setLeftStart(sf::Sprite& sprite, sf::Vector2f& dir, float& speed);
void setRightStart(sf::Sprite& sprite, sf::Vector2f& dir, float& speed);
void setDir(sf::Sprite& sprite, SIDE side);
void setDir(sf::Sprite& sprite, sf::Vector2f& dir, SIDE side);
void drawAllSprite(sf::RenderWindow& window, sf::Sprite* sprites, int size = 1, SIDE* side = nullptr);
void updateBranch(SIDE* side, int size);
bool isCollision(SIDE side1, SIDE side2);
void textInit(sf::Text& text, sf::Font& font, int fontsize, std::string word = " ", sf::Vector2f origin = { 0,0 }, sf::Vector2f pos = { 0 , 0 });
float deltaTime;

int main() {

	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Timber Game!");
	windowSize = { (float)window.getSize().x  , (float)window.getSize().y };
	srand(time(0));


#pragma region Texture
	sf::Texture textureBackGround;
	sf::Texture textureCloud;
	sf::Texture textureTree;
	sf::Texture texturePlayer;
	sf::Texture textureBranch;
	sf::Texture textureAxe;
	sf::Texture textureLog;

	textureBackGround.loadFromFile(graphicsPath + "background.png");
	textureCloud.loadFromFile(graphicsPath + "cloud.png");
	textureTree.loadFromFile(graphicsPath + "tree.png");
	texturePlayer.loadFromFile(graphicsPath + "player.png");
	textureBranch.loadFromFile(graphicsPath + "branch.png");
	textureAxe.loadFromFile(graphicsPath + "axe.png");
	textureLog.loadFromFile(graphicsPath + "log.png");
#pragma endregion

#pragma region Fonts
	sf::Font font;

	font.loadFromFile(fontPath + "KOMIKAP_.ttf");
#pragma endregion

#pragma region Directions
	sf::Vector2f dirCloud = { 1 , 0 };
	sf::Vector2f dirLog = { 1,  -1 };
#pragma endregion

#pragma region Rectangle
	sf::RectangleShape timer;
	
	timer.setSize({ TIMTER_SIZE_WIDTH, TIMER_SIZE_HEIGHT});
	timer.setFillColor(sf::Color::Red);
	timer.setPosition({ windowSize.x / 2 - 250 , windowSize.y - 150 });
#pragma endregion


#pragma region Speeds
	float speedCloud = 500;
	float speedLog = 1000;
#pragma endregion

#pragma region Directions
	SIDE sidePlayer = SIDE::LEFT;
	SIDE sideBranch[6] = { SIDE::LEFT , SIDE::RIGHT ,SIDE::LEFT , SIDE::RIGHT , SIDE::NONE , SIDE::NONE };
	SIDE sideAxe = SIDE::LEFT;
	SIDE sideLog[NUM_LOGS] = { SIDE::NONE,SIDE::NONE,SIDE::NONE,SIDE::NONE,SIDE::NONE,SIDE::NONE,SIDE::NONE,SIDE::NONE,SIDE::NONE,SIDE::NONE };
#pragma endregion

#pragma region Sprite
	sf::Sprite spriteBackGround;
	sf::Sprite spriteCloud;
	sf::Sprite spriteTree;
	sf::Sprite spritePlayer;
	sf::Sprite spriteBranch[NUM_BRANCHS];
	sf::Sprite spriteAxe;
	sf::Sprite spriteLog[NUM_LOGS];

	spriteInit(spriteBackGround, textureBackGround);
	spriteInit(spriteCloud, textureCloud);
	spriteInit(spriteTree, textureTree, { (float)(textureTree.getSize().x / 2) , 0.f }, { windowSize.x / 2 , .0f });
	spriteInit(spritePlayer, texturePlayer, { -(float)(textureTree.getSize().x / 2) , 0.f }, { spriteTree.getPosition().x , 700.f });
	for (int i = 0; i < NUM_BRANCHS; i++) {
		spriteInit(spriteBranch[i], textureBranch, { -(float)(textureTree.getSize().x / 2) , 0.f }, { spriteTree.getPosition().x , i * 150.f });
	}
	spriteInit(spriteAxe, textureAxe, { -(float)(texturePlayer.getSize().x * 0.1f) , 0.f }, { spritePlayer.getPosition().x, spritePlayer.getPosition().y + 110.f });
	for (int i = 0; i < NUM_LOGS; i++) {
		spriteInit(spriteLog[i], textureLog, { (float)(textureLog.getSize().x / 2), (float)textureLog.getSize().y }, { windowSize.x / 2 , (float)textureTree.getSize().y });
	}

#pragma endregion

#pragma region Text
	sf::Text textScore;

	textInit(textScore, font, 100, "SCORE = 0", { 0,0 }, { 20, 20 });
#pragma endregion

	sf::Event event;
	sf::Clock clock;
	bool isDownKey = false;
	bool stopGame = false;
	bool hit = false;
	bool isActive[NUM_LOGS] = { false , false , false , false , false , false , false , false , false , false };
	int score = 0;
	int idx = 0;
	float setTimer = 5.f;
	sf::Vector2f gravity = { 0.f , 5000.f };
	sf::Vector2f velocity[NUM_LOGS] = { dirLog * speedLog , dirLog * speedLog , dirLog * speedLog , dirLog * speedLog  , dirLog * speedLog  , dirLog * speedLog  , dirLog * speedLog  , dirLog * speedLog  ,dirLog * speedLog  ,dirLog * speedLog };
	sf::Vector2f initPostionLog = spriteLog[0].getPosition();


	while (window.isOpen()) {
		sf::Time time = clock.restart();
		deltaTime = time.asSeconds();
		
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (setTimer <= 0) {
				stopGame = true;
			}
			if (event.key.code == sf::Keyboard::Enter) {
				stopGame = false;
				hit = false;
				setTimer = 5;
				textScore.setString("SCORE = " + std::to_string(score));
			}
			if (stopGame) continue;

			if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) && !isDownKey)
			{

				for (int i = 0; i < NUM_LOGS; i++) {
					if (!isActive[i]) {
						idx = i;
						isActive[i] = true;
						break;
					}
				}

				if (event.key.code == sf::Keyboard::Left) {
					sidePlayer = SIDE::LEFT;
					sideAxe = SIDE::LEFT;
					sideLog[idx] = SIDE::LEFT;
					dirLog = { 1 , -1 };
				}
				else if (event.key.code == sf::Keyboard::Right) {
					sidePlayer = SIDE::RIGHT;
					sideAxe = SIDE::RIGHT;
					sideLog[idx] = SIDE::RIGHT;
					dirLog = { -1 , -1 };
				}

				velocity[idx] = dirLog * speedLog;
				printf("%f\n", velocity[idx].x);
				spriteLog[idx].setPosition(initPostionLog);

				updateBranch(sideBranch, NUM_BRANCHS);
				if (isCollision(sidePlayer, sideBranch[NUM_BRANCHS - 1])) {
					stopGame = true;
					score = 0;
				}
				else {
					score += 10;
					textScore.setString("SCORE = " + std::to_string(score));
				}
				isDownKey = true;
				hit = true;

				
			}
			if (event.type == sf::Event::KeyReleased) {
				isDownKey = false;
			}
		}


		if (!stopGame) {
			moveObject(spriteCloud, dirCloud, speedCloud);
			if (checkOutOfWindow(spriteCloud)) {
				resetObject(spriteCloud, dirCloud, speedCloud);
			}

			for (int i = 0; i < NUM_LOGS; i++) {
				if (isActive[i]) {
					sf::Vector2f pos = spriteLog[i].getPosition();
					velocity[i] += gravity * deltaTime;
					pos += velocity[i] * deltaTime;
					spriteLog[i].setPosition(pos);
					if (checkOutOfWindow(spriteLog[i])) isActive[i] = false;
				}
				else {
					sideLog[i] = SIDE::NONE;
				}
			}

			setTimer -= deltaTime;
			if (setTimer <= 0) setTimer = 0;
			timer.setSize({ (float)TIMTER_SIZE_WIDTH * (setTimer / 5.f) , timer.getSize().y });
			
		}


		//setDir(spritePlayer, sidePlayer);

		window.clear();
		window.draw(spriteBackGround);
		window.draw(spriteCloud);
		window.draw(spriteTree);
		drawAllSprite(window, &spritePlayer, 1, &sidePlayer);
		drawAllSprite(window, spriteBranch, NUM_BRANCHS, sideBranch);
		if (isDownKey) drawAllSprite(window, &spriteAxe, 1, &sideAxe);
		drawAllSprite(window, spriteLog, NUM_LOGS, sideLog);
		window.draw(textScore);
		window.draw(timer);
		window.display();


	}
	return 0;
}
	

	void spriteInit(sf::Sprite & sprite, sf::Texture & texture, sf::Vector2f origin, sf::Vector2f position) {
		sprite.setTexture(texture);
		sprite.setPosition(position);
		sprite.setOrigin(origin);
	}

	void moveObject(sf::Sprite & sprite, sf::Vector2f dir, float speed) {
		sf::Vector2f pos = sprite.getPosition();
		pos += dir * speed * deltaTime;
		sprite.setPosition(pos);
	}



	void setDir(sf::Sprite & sprite, sf::Vector2f & dir, SIDE side) {
		if (side == SIDE::LEFT) {
			sprite.setScale({ -1 , 1 });
			dir = { -1 , dir.y };
		}
		else if (side == SIDE::RIGHT) {
			sprite.setScale({ 1 , 1 });
			dir = { 1 , dir.y };
		}
	}

	void drawAllSprite(sf::RenderWindow & window, sf::Sprite * sprites, int size, SIDE * side)
	{
		for (int i = 0; i < size; i++) {
			setDir(sprites[i], side[i]);
			if (side == nullptr || side[i] != SIDE::NONE) {
				window.draw(sprites[i]);
			}

		}
	}

	void updateBranch(SIDE * sides, int size)
	{
		for (int i = size - 1; i >= 0; i--) {
			sides[i] = sides[i - 1];
		}
		int random = rand() % 3;
		if (random == 0) sides[0] = SIDE::LEFT;
		else if (random == 1) sides[0] = SIDE::RIGHT;
		else if (random == 2) sides[0] = SIDE::NONE;

	}

	bool isCollision(SIDE side1, SIDE side2)
	{
		if (side1 == side2) {
			return true;
		}
		return false;
	}

	void textInit(sf::Text & text, sf::Font & font, int fontsize, std::string word, sf::Vector2f origin, sf::Vector2f pos)
	{
		text.setFont(font);
		text.setCharacterSize(fontsize);
		text.setString(word);
		text.setOrigin(origin);
		text.setPosition(pos);
	}

	void setDir(sf::Sprite & sprite, SIDE side) {
		if (side == SIDE::LEFT) {
			sprite.setScale({ -1 , 1 });

		}
		else if (side == SIDE::RIGHT) {
			sprite.setScale({ 1 , 1 });
		}
	}

	void resetObject(sf::Sprite & sprite, sf::Vector2f & dir, float& speed) {
		int random = rand() % 2;

		if (random == 0) {
			setLeftStart(sprite, dir, speed);
			setDir(sprite, dir, SIDE::RIGHT);
		}
		else {
			setRightStart(sprite, dir, speed);
			setDir(sprite, dir, SIDE::LEFT);
		}
	}

	void setLeftStart(sf::Sprite & sprite, sf::Vector2f & dir, float& speed) {
		sprite.setPosition(-100, sprite.getPosition().y);
	}

	void setRightStart(sf::Sprite & sprite, sf::Vector2f & dir, float& speed) {
		sprite.setPosition(windowSize.x + 100, sprite.getPosition().y);
	}

	bool checkOutOfWindow(sf::Sprite & sprite) {
		if (sprite.getPosition().x < -150 || sprite.getPosition().x > windowSize.x + 150 || sprite.getPosition().y < -150 || sprite.getPosition().y > windowSize.y + 150) {
			return true;
		}

		return false;
	}



