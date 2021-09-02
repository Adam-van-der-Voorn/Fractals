#include "EditingGUI.h"
#include "Editing.h"
#include "EditingState.h"
#include "EditableLine.h"
#include "EditableLineNode.h"
#include "debug_printing.h"
#include "StateMachine.h"
#include "SelLineWidget.h"
#include "LineFractal.h"
#include "WidgetHoriStack.h"
#include "AbsLine.h"
#include "vecutil.h"
#include "sfml_conversions.h"
#include "pi.h"
#include "FrameState.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <memory>

EditingGUI::EditingGUI(EditingState* state, Editing* data)
	: state(state), editing(data)
{

	// set up baseline
	const std::vector<const EditableLineNode*>& nodes = editing->getFrame()->getBaseLine().getNodes();
	for (int i = 0; i < 2; i++) {
		baseLine[i].position.x = nodes[i]->getPosition().x;
		baseLine[i].position.y = nodes[i]->getPosition().y;
		baseLine[i].color = sf::Color(80, 80, 80);
	}

	updateFractal(data->getFrame());

	tGui = std::make_shared<tgui::Gui>(*state->getRenderWindow());
	setupTGUI(state->getRenderWindow()->getSize().x, state->getRenderWindow()->getSize().y);
}

void EditingGUI::onNotify(Editing* e, int event_num)
{
	auto event = static_cast<Editing::Event>(event_num);
	const FrameState* frame = e->getFrame();
	switch (event)
	{
	case Editing::LINES_CHANGED:
		updateLines(frame);
		updateNodes(frame);
		break;
	case Editing::SELECTION_CHANGED:
		updateNodes(frame);
		{
			std::vector<tgui::Widget::Ptr> widgets;
			for (NodeID selected_node_id : e->getFrame()->getSelectedNodes()) {
				auto widget = std::make_shared<SelLineWidget>(editing, selected_node_id, editing->general_element_width);
				widgets.push_back(widget);
			}
			node_selections->swapStack(widgets);
		}
		break;
	case Editing::FRACTAL_CHANGED:
		updateFractal(frame);
		break;
	case Editing::MOUSE_MOVED:
		updateNodes(frame);
		break;
	case Editing::HOVERED_NODE_CHANGED:
		updateNodes(frame);
		break;
	default:
		break;
	}
}

void EditingGUI::updateNodes(const FrameState* frame)
{
	assert(frame->getNodes().size() % 2 == 0 && "uneven amount of nodes");
	nodes.resize(frame->getNodes().size());
	int i = 0;
	for (const auto& node_pair : frame->getNodes()) {
		nodes[i].setRadius(EditableLineNode::NODE_RADIUS);
		nodes[i].setFillColor(sf::Color::Transparent);
		nodes[i].setOutlineThickness(1.0f);
		nodes[i].setOrigin(sf::Vector2f(EditableLineNode::NODE_RADIUS, EditableLineNode::NODE_RADIUS));

		nodes[i].setPosition(sf::Vector2f(node_pair.second->getPosition().x, node_pair.second->getPosition().y));

		if (editing->getHoveredNode() == node_pair.first) { // node is selected and hovered
			nodes[i].setOutlineColor(sf::Color::Green);
		}
		else if (frame->getSelectedNodes().count(node_pair.first)) { // node is selected
			nodes[i].setOutlineColor(sf::Color::Red);
		}
		else if (node_pair.second->pointIntersection(editing->getMousePosInFrame() - editing->getGlobalOffset())) {
			nodes[i].setOutlineColor(sf::Color::White);
		}
		else {
			nodes[i].setOutlineColor(sf::Color::Transparent);
		}
		i++;
	}
}

void EditingGUI::updateLines(const FrameState* frame)
{
	assert(frame->getNodes().size() % 2 == 0 && "uneven amount of nodes");
	int old_vertcount = nodeLines.getVertexCount();
	int new_vertcount = frame->getLines().size() * 6; // 6 bc two verts each on three lines making up an arrow.
	nodeLines.resize(new_vertcount);
	if (new_vertcount > old_vertcount) {
		for (int i = old_vertcount; i < new_vertcount; i++) {
			nodeLines[i].color = LINE_COL;
		}
	}
	int i = 0;
	for (const auto& line_pair : frame->getLines()) {
		AbsLine line = line_pair.second.toAbsLine();
		sf::Vector2f sfBack = sfVecFromVec2(line.back);
		sf::Vector2f sfHead = sfVecFromVec2(line.head);
		// centerline
		nodeLines[i].position = sfBack;
		i++;
		nodeLines[i].position = sfHead;
		i++;

		// arrowheads
		constexpr double arrowhead_len = EditableLineNode::NODE_RADIUS;
		const double arrow_dir = line.angle();
		nodeLines[i].position = sfHead;
		i++;
		nodeLines[i].position.x = line.head.x + lendirX(arrowhead_len, arrow_dir + m_pi4*3);
		nodeLines[i].position.y = line.head.y + lendirY(arrowhead_len, arrow_dir + m_pi4*3);
		i++;
		nodeLines[i].position = sfHead;
		i++;
		nodeLines[i].position.x = line.head.x + lendirX(arrowhead_len, arrow_dir - m_pi4*3);
		nodeLines[i].position.y = line.head.y + lendirY(arrowhead_len, arrow_dir - m_pi4*3);
		i++;
	}
}

void EditingGUI::updateFractal(const FrameState* frame) {
	int j = 0;
	const std::vector<AbsLine>& fractal_lines = frame->getFractal();
	fractal.resize(fractal_lines.size() * 2);
	for (size_t i = 0; i < fractal.getVertexCount(); i += 2) {
		fractal[i] = sf::Vertex(sf::Vector2f(fractal_lines[j].back.x, fractal_lines[j].back.y));
		fractal[i + 1] = sf::Vertex(sf::Vector2f(fractal_lines[j].head.x, fractal_lines[j].head.y));
		j++;
	}
}

void EditingGUI::realignTGUI(int window_width, int window_height)
{
	right_panel->setSize(editing->right_panel_width, window_height);
	right_panel->setPosition(window_width - editing->right_panel_width, 0);
	display_button->setPosition(editing->general_padding, window_height - editing->general_padding - display_button->getSize().y);
	node_selections->setSize(editing->general_element_width, display_button->getPosition().y - (2 * editing->general_padding) - (line_actions_field->getPosition().y + line_actions_field->getSize().y));
}

void EditingGUI::setupTGUI(int window_width, int window_height)
{
	// right panel
	right_panel = tgui::Panel::create();
	tGui->add(right_panel);
	right_panel->setInheritedOpacity(1);

	// display button
	display_button = tgui::Button::create();
	right_panel->add(display_button);
	display_button->setSize(editing->general_element_width, 50);
	display_button->onClick(&StateMachine::changeState, state->getStateMachine(), "viewing");
	display_button->setText("Display fractal");

	// recursions field
	recursions_field = tgui::Panel::create();
	right_panel->add(recursions_field);
	recursions_field->setSize(editing->general_element_width, 20);
	recursions_field->setPosition(editing->general_padding, editing->general_padding);

	// recursions label
	recursions_label = tgui::Label::create();
	recursions_field->add(recursions_label);
	recursions_label->setSize({ "100%", "100%" });
	recursions_label->setPosition(0, 3); // slight adjustment so it lines up with input text
	recursions_label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Left);
	recursions_label->setText("Number of recursions");

	// recursions input
	recursions_input = tgui::EditBox::create();
	recursions_field->add(recursions_input);
	recursions_input->setSize({ 33, "100%" });
	recursions_input->setPosition({ recursions_field->getSize().x - recursions_input->getSize().x, 0 });
	recursions_input->setText(std::to_string(editing->getNumRecursions()));
	recursions_input->onReturnKeyPress(&EditingGUI::changeRecursionsField, this);

	// line actions field
	line_actions_field = tgui::Panel::create();
	right_panel->add(line_actions_field);
	line_actions_field->setSize(editing->general_element_width, 40);
	line_actions_field->setPosition({ editing->general_padding, tgui::bindBottom(recursions_field) + editing->general_padding });

	// add line button
	add_line_button = tgui::Button::create();
	line_actions_field->add(add_line_button);
	add_line_button->setSize((line_actions_field->getSize().x - editing->general_padding) / 2, "100%");
	add_line_button->setPosition(0, 0);
	add_line_button->setText("add line");
	add_line_button->onClick(&Editing::newLine, editing);

	// remove line button
	remove_line_button = tgui::Button::create();
	line_actions_field->add(remove_line_button);
	remove_line_button->setSize((editing->general_element_width - editing->general_padding) / 2, "100%");
	remove_line_button->setPosition({ tgui::bindRight(add_line_button) + editing->general_padding, 0 });
	remove_line_button->setText("remove line");
	remove_line_button->onClick(&Editing::removeSelectedLines, editing);

	// node selections
	node_selections = std::make_shared<WidgetHoriStack>();
	node_selections->setMargins(0, editing->general_padding);
	right_panel->add(node_selections);

	node_selections->setPosition({ editing->general_padding, tgui::bindBottom(line_actions_field) + editing->general_padding });

	realignTGUI(window_width, window_height);
}

void EditingGUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(fractal, sfTransformFromVec2(editing->getGlobalOffset()));
	target.draw(baseLine, sfTransformFromVec2(editing->getGlobalOffset()));
	target.draw(nodeLines, sfTransformFromVec2(editing->getGlobalOffset()));
	for (auto& node : nodes) {
		target.draw(node, sfTransformFromVec2(editing->getGlobalOffset()));
	}
	tGui->draw();

	if (DEBUG) {
		sf::RectangleShape tmp;
		tmp.setOutlineColor(sf::Color::Magenta);
		tmp.setOutlineThickness(1.0f);
		tmp.setFillColor(sf::Color::Transparent);
		tmp.setPosition(editing->getMousePosInFrame().x, editing->getMousePosInFrame().y);
		tmp.setSize({10, 10});
		target.draw(tmp);
	}
}

void EditingGUI::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::Resized) {
		realignTGUI(event.size.width, event.size.height);
	}
	tGui->handleEvent(event);
}

void EditingGUI::changeRecursionsField() {
	tgui::String new_val = recursions_input->getText();
	if (new_val.length() == 0) {
		recursions_input->setText(std::to_string(editing->getNumRecursions()));
	}
	else {
		int new_num = new_val.toInt();
		if (new_num != editing->getNumRecursions()) {
			constexpr int max_val = 9999;
			if (new_num > max_val) {
				new_num = max_val;
				recursions_input->setText(std::to_string(new_num));
			}
			editing->setNumRecursions(new_num);
			editing->remapFractal();
		}
	}
}

