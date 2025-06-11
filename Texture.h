#pragma once
#include <SFML/Graphics.hpp>
class Texture
{
public:
	sf::Texture texture;
	sf::Sprite sprite;

	Texture(const std::string path);
	~Texture();
};

