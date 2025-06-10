#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    sf::RenderWindow window(sf::VideoMode(300, 300), "abs"); // 화면에 뛰울 윈도우의 사이즈 / 제목
    sf::CircleShape shape(50.f);  // 화면에 그릴 원의 반지름
    shape.setFillColor(sf::Color::Green); // 그림을 그린 shape의 색깔

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        
        
        window.clear(); 
        window.draw(shape);
        window.display();
    }

    return 0;
}