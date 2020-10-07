#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class Drawable {
public: 
	virtual void draw(sf::RenderWindow* window) const = 0;
};