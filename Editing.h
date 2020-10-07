#pragma once
#include "State.h"
#include <TGUI/TGUI.hpp>
class Editor : public State
{
public:
	Editor(sf::RenderWindow* window);
	void draw(sf::RenderWindow* window) const;
private:
	tgui::Button::Ptr test_button = tgui::Button::create();
};

