#include "GameObject.h"

GameObject::GameObject(const std::string path)
{
	texture.loadFromFile(path);
	sprite.setTexture(texture);
}

GameObject::~GameObject()
{
}

void GameObject::flipX(bool flip)
{
	if (flipX == flip) return;
	
	if (flip) {
		
	}
}
