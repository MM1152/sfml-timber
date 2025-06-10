#include <SFML/Graphics.hpp>





int main()
{
    sf::RenderWindow window(sf::VideoMode(1920 , 1080), "Timber!"); // 화면에 뛰울 윈도우의 사이즈 / 제목

    sf::Texture textureBackGround; // background Image 로딩용
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