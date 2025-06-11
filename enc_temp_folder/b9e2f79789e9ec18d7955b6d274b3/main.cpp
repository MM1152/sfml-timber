#include <SFML/Graphics.hpp>
#include <iostream>
#include "Texture.h"
#include <Windows.h>

int main()
{
    const std::string resourcePath = "Resources/";
    const std::string graphicsPath = resourcePath + "graphics/";

    sf::RenderWindow window(sf::VideoMode(1920 , 1080), "Timber!"); // 화면에 뛰울 윈도우의 사이즈 / 제목

    sf::Texture textureCloud;
    sf::Texture textureBee;
    sf::Texture textureTree;
    sf::Texture textureBackGround;

    textureCloud.loadFromFile(graphicsPath + "cloud.png");
    textureBee.loadFromFile(graphicsPath + "bee.png");
    textureTree.loadFromFile(graphicsPath + "tree.png");
    textureBackGround.loadFromFile(graphicsPath + "background.png");

    sf::Sprite spriteBackGround;
    sf::Sprite spriteCloud;
    sf::Sprite spriteBee;
    sf::Sprite spriteTree;
    
    spriteBackGround.setTexture(textureBackGround);
    spriteCloud.setTexture(textureCloud);
    spriteTree.setTexture(textureTree);
    spriteBee.setTexture(textureBee);

    spriteTree.setPosition(1920 * 0.5 , 0);
    spriteTree.setOrigin(textureTree.getSize().x * 0.5f, 0.f);
    
    spriteBee.setPosition(textureBee.getSize().x / 2, 1080 * 0.7 - textureBee.getSize().y / 2);
    spriteBee.setOrigin(textureBee.getSize().x / 2, textureBee.getSize().y / 2);

    spriteCloud.setOrigin(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2);

    std::cout << window.getSize().x << std::endl;
    sf::Vector2f pos;
    sf::Event event;
    while (window.isOpen())
    {   
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(spriteBackGround);
        window.draw(spriteTree);
        Sleep(1000);
        spriteCloud.setPosition(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2);
        window.draw(spriteCloud);
        Sleep(1000);
        spriteCloud.setPosition(textureCloud.getSize().x / 2 , textureCloud.getSize().y / 2 + textureCloud.getSize().y);
        window.draw(spriteCloud);
        Sleep(1000);
        spriteCloud.setPosition(textureCloud.getSize().x / 2, textureCloud.getSize().y / 2 + textureCloud.getSize().y * 2);
        window.draw(spriteCloud);
        Sleep(1000);
        window.draw(spriteBee);
        Sleep(1000);
        window.display();
    }

    return 0;
}