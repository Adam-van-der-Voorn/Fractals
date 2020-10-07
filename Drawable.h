#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

class Drawable {
public: 
	virtual void drawTo(sf::RenderTarget& surface) const = 0;
};