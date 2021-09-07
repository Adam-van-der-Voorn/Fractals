#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <TGUI/TGUI.hpp>
#include "Observer.h"
#include "EditingState.h"
#include "EventHandler.h"
#include "WidgetHoriStack.h"

class Editing;
class EditableLineNode;
class FrameState;

class EditingGUI : public sf::Drawable, public Observer<Editing>, public EventHandler
{
public:
	EditingGUI(EditingState* state, Editing* data);

	// inherited via Observer
	void onNotify(Editing* editing, int event_num) override;

	// inherited via sf::Drawable
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;	

	// inherited via EventHandler
	void handleEvent(sf::Event& event) override;

private:
	void updateNodes(const FrameState* frame);
	void updateLines(const FrameState* frame);
	void updateFractal(const FrameState* frame);

	void realignTGUI(int window_width, int window_height);
	void setupTGUI(int window_width, int window_height);

	const sf::Color LINE_COL = sf::Color::Blue;
	const sf::Color LINE_COL_UNRECURSIVE = sf::Color::Red;

	Editing* editing;
	EditingState* state;
	sf::VertexArray nodeLines = sf::VertexArray(sf::Lines);
	sf::VertexArray baseLine = sf::VertexArray(sf::Lines, 2);
	std::vector<sf::CircleShape> nodes;
	sf::VertexArray fractalLines = sf::VertexArray(sf::Lines);

	sf::VertexArray fractal = sf::VertexArray(sf::Lines);


	std::shared_ptr<tgui::Gui> tGui;
	tgui::Panel::Ptr right_panel;
	tgui::Button::Ptr display_button;
	tgui::Panel::Ptr line_actions_field;
	tgui::Button::Ptr add_line_button;
	tgui::Button::Ptr remove_line_button;
	tgui::Panel::Ptr recursions_field;
	tgui::EditBox::Ptr recursions_input;
	tgui::Label::Ptr recursions_label;
	WidgetHoriStack::Ptr node_selections;
};

