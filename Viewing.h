#pragma once
#include <vector>
#include <memory>
#include <TGUI/TGUI.hpp>
#include "State.h"

class LineFractal;
class ZoomBox;
class StateMachine;

class Viewing : public State
{
public:
	Viewing(sf::RenderWindow& window);
	~Viewing();

	void enter();
	void run();
	void exit();
	void handleEvent(sf::Event& event);

	void addFractal(LineFractal* fractal);
	void drawTo(sf::RenderTarget& surface) const;

	void incFractal();


	//getters & setters
	ZoomBox* getZoomBox() const;

private:
	std::vector<LineFractal*> fractals;
	ZoomBox* zoom_box;

	// GUI elements
	tgui::Gui* gui;
};

