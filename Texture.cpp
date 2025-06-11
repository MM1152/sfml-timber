#include "Texture.h"


Texture::Texture(const std::string path)
{
	texture.loadFromFile(path);
	sprite.setTexture(texture);
}

Texture::~Texture()
{

}
