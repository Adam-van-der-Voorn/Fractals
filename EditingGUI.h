#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <TGUI/TGUI.hpp>
#include "Observer.h"

class Editing;
class EditableLineNode;

class EditingGUI : public sf::Drawable, Observer
{
public:
	EditingGUI(Editing* editing, sf::RenderWindow& window);
	void onNotify(int event_num);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void handleEvent(sf::Event& event);
private:
	void updateNodes();
	void updateLines();
	void changeRecursionsField();

	void realignTGUI(int window_width, int window_height);
	void setupTGUI(int window_width, int window_height);

	Editing* editing;
	sf::VertexArray nodeLines = sf::VertexArray(sf::Lines);
	sf::VertexArray baseLine = sf::VertexArray(sf::Lines, 2);
	std::vector<sf::CircleShape> nodes;
	sf::VertexArray fractalLines = sf::VertexArray(sf::Lines);


	std::shared_ptr<tgui::Gui> tGui;
	tgui::Panel::Ptr right_panel;
	tgui::Button::Ptr display_button;
	tgui::Panel::Ptr line_actions_field;
	tgui::Button::Ptr add_line_button;
	tgui::Button::Ptr remove_line_button;
	tgui::Panel::Ptr recursions_field;
	tgui::EditBox::Ptr recursions_input;
	tgui::Label::Ptr recursions_label;
	tgui::Panel::Ptr measurement_fields[3];
	tgui::EditBox::Ptr measurement_inputs[3];
	tgui::Label::Ptr measurement_labels[3];
	tgui::ScrollablePanel::Ptr node_selections;
};

