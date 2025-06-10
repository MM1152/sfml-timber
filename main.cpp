#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    sf::RenderWindow window(sf::VideoMode(300, 300), "abs"); // ȭ�鿡 �ٿ� �������� ������ / ����
    sf::CircleShape shape(50.f);  // ȭ�鿡 �׸� ���� ������
    shape.setFillColor(sf::Color::Green); // �׸��� �׸� shape�� ����

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