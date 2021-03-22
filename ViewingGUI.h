#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <TGUI/TGUI.hpp>
#include "EventHandler.h"
#include "Observer.h"

class ViewingState;
class Viewing;

class ViewingGUI : public sf::Drawable, public EventHandler, public Observer<Viewing>
{
public:
	ViewingGUI(ViewingState* state, Viewing* data);

	// Inherited via EventHandler
	void handleEvent(sf::Event& event);

	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	Updates the fractal graphics to match the fractal data
	**/
	void updateFractal();


private:


	Viewing* data;
	
	sf::VertexArray fractal_lines = sf::VertexArray(sf::Lines);
	sf::RectangleShape zoom_box_shape = sf::RectangleShape();

	//tgui stuff
	std::unique_ptr<tgui::Gui> gui;
	tgui::Button::Ptr test;

	// Inherited via Observer<Viewing>
	void onNotify(Viewing* subject, int event_num) override;
};

