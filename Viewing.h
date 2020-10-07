#pragma once
#include <vector>
#include <memory>
#include <TGUI/TGUI.hpp>
#include "State.h"
#include "Drawable.h"

class LineFractal;
class ZoomBox;

class Viewing : public State, public Drawable
{
public:
	Viewing(sf::RenderWindow* window);
	~Viewing();

	void enter();
	void run();
	void exit();
	void handleEvent(sf::Event& event);

	void addFractal(LineFractal* fractal);
	void draw(sf::RenderWindow* window) const;

	void incFractal();


	//getters & setters
	ZoomBox* getZoomBox() const;

private:
	std::vector<LineFractal*> fractals;
	ZoomBox* zoom_box;

	// GUI elements
	tgui::Gui* gui;
};

