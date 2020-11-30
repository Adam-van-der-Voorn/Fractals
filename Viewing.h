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

	//inherited via state
	void enter();
	void run();
	void exit();
	void handleEvent(sf::Event& event);
	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	void addFractal(LineFractal* fractal);

	void incFractal();


	//getters & setters
	ZoomBox* getZoomBox() const;

private:
	std::vector<LineFractal*> fractals;
	ZoomBox* zoom_box;

	// GUI elements
	tgui::Gui* gui;
};

