#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	sf::Vector2f scale;
	bool flipX;

	GameObject(const std::string path);
	~GameObject();
	
	void flipX(bool flip);
};

