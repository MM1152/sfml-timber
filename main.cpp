#include <SFML/Graphics.hpp>





int main()
{
    sf::RenderWindow window(sf::VideoMode(1920 , 1080), "Timber!"); // ȭ�鿡 �ٿ� �������� ������ / ����

    sf::Texture textureBackGround; // background Image �ε���
    textureBackGround.loadFromFile("Resources/graphics/background.png");

    sf::Sprite spriteBackGround;
    spriteBackGround.setTexture(textureBackGround);

    while (window.isOpen())
    {
        sf::Event event;


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(); 
        window.draw(spriteBackGround);
        window.display();
    }

    return 0;
}