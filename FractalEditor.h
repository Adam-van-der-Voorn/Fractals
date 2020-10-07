#pragma once
#include "Drawable.h"
#include <SFML/Graphics/RenderWindow.hpp>

class LineFractal;

class FractalEditor : public Drawable
{
public:
	FractalEditor(LineFractal& fractal);
	void draw(sf::RenderWindow* drawable) const;
};

