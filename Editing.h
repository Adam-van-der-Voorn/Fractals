#pragma once
#include "State.h"
#include <unordered_map>
#include <TGUI/TGUI.hpp>

class EditableLine;

class Editing : public State
{
public:
	Editing(sf::RenderWindow* window);
	~Editing();

	void enter();
	void run();
	void exit();
	void handleEvent(sf::Event& event);

	void drawTo(sf::RenderTarget& surface) const;
private:
	std::unordered_map<int, EditableLine> lines;

	// gui stuff
	void realignGUI(int window_width, int window_height);
	void setupGUI(int window_width, int window_height);

	sf::Vector2i editing_frame_center;

	tgui::Gui* gui;
	tgui::Panel::Ptr right_panel;
	tgui::Button::Ptr display_button;
	tgui::Button::Ptr add_line_button;
	tgui::Button::Ptr remove_line_button;
	tgui::Panel::Ptr recursions_field;
	tgui::EditBox::Ptr recursions_input;
	tgui::Label::Ptr recursions_label;
	tgui::Panel::Ptr measurements_block;
	tgui::EditBox::Ptr measurement_inputs[3];
	tgui::Label::Ptr measurement_labels[3];

	const int right_panel_width = 200;
	const int general_padding = 5;
	const int general_element_width = right_panel_width - (general_padding * 2);
	const int display_button_height = 50;
	const int add_remove_button_height = 40;
	const int recursions_field_height = 20;
	const int recursions_input_width = 33;
	const int measurements_block_height = 100;
	const int no_measurement_fields = sizeof(measurement_inputs) / sizeof(tgui::EditBox::Ptr);
	const int measurements_field_height = (measurements_block_height - ((no_measurement_fields + 1) * general_padding)) / no_measurement_fields;
	const int measurements_input_width = 120;
};

