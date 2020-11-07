#pragma once
#include "State.h"
#include "Subject.h"
#include <SFML/Graphics/Drawable.hpp>
#include <unordered_map>
#include <unordered_set>
#include <TGUI/TGUI.hpp>
#include <memory>


class EditableLineNode;
class EditableLine;
class EditingGUI;
class LineFractal;
class StateMachine;

class Editing : public State
{
public:
	Editing(sf::RenderWindow& window, LineFractal* fractal, StateMachine* state_machine);
	~Editing();

	void enter();
	void run();
	void exit();
	void handleEvent(sf::Event& event);

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	const std::unordered_map<int, std::shared_ptr<EditableLineNode>>& getNodes() const;
	const std::unordered_map<int, std::shared_ptr<EditableLine>>& getLines() const;
	const std::shared_ptr<EditableLine> getBaseLine() const;
	const std::unordered_set<int>& getSelectedNodes() const;

	void addObserver(Observer* observer);

	enum Event{
		LINES_CHANGED, SELECTION_CHANGED
	};
private:
	void fractalChanged();
	void addLine();
	void changeRecursionsField();

	void notifyAll(Event e) const;
	bool isWithinEditingFrame(sf::Vector2f point) const;


	std::unordered_set<Observer*> observers;
	std::unordered_map<int, std::shared_ptr<EditableLineNode>> nodes;
	std::unordered_set<int> selected_nodes;
	std::unordered_set<int> dragging_nodes;
	std::unordered_map<int, std::shared_ptr<EditableLine>> lines;
	std::shared_ptr<EditableLine> base_line;
	LineFractal* fractal;
	StateMachine* state_machine;

	int num_recursions = 6;
	bool mouse_moved_since_lpress = false;
	sf::Vector2i left_press_location;

	// gui stuff
	std::shared_ptr<EditingGUI> gui_2;

	void realignGUI(int window_width, int window_height);
	void setupGUI(int window_width, int window_height);

	sf::Vector2i editing_frame_size;
	sf::Vector2i editing_frame_center;
	sf::Vector2i mouse_framepos;

	tgui::Gui* gui;
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

	const int right_panel_width = 200;
	const int general_padding = 6;
	const int general_element_width = right_panel_width - (general_padding * 2);
};

