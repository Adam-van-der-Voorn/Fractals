#include "Editing.h"
#include "StateMachine.h"
#include "LineFractal.h"
#include "debug_printing.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "vertarrutil.h"
#include "AbsLine.h"
#include "EditableLineNode.h"
#include "SelLineWidget.h"
#include "EditingGUI.h"
#include "EditingState.h"
#include "Vec2.h"
#include "sfml_conversions.h"
#include "FrameState.h"
#include <cstdlib>
#include <string>
#include <memory>

const AbsLine inital_base_line = { {-200, 0}, {+200, 0} };

Editing::Editing(EditingState* state) :
	state(state) , fractalGen(inital_base_line)
{
	// set seed for line creation
	srand(132);

	// setup base line
	recalcEditingFrameDimensions(vec2FromSF(state->getRenderWindow()->getSize()));
	fractalGen.setView(RightAngleRect::fromSize({0, 0}, editing_frame_size));
	frame_stack.push_back(FrameState(inital_base_line));
	frame = &frame_stack.back();
	frame->setFractal(fractalGen.generate());
	
	//setup hovered node
	clearHoveredNode();
}

const LineFractal& Editing::getFractalGenerator() const
{
	return fractalGen;
}

FrameState* Editing::getFrame() 
{
	return frame;
}

void Editing::setLineRecursiveness(int line_id, bool b)
{
	frame->setLineRecursiveness(line_id, b);
	remapFractal();
	notifyAll(Event::LINES_CHANGED);
}

Vec2 Editing::getGlobalOffset() const
{
	return global_offset;
}

void Editing::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::Resized) {
		recalcEditingFrameDimensions({ static_cast<double>(event.size.width), static_cast<double>(event.size.height) });
	}
	else if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Z) {
			/* TODO REIMPLEMENT UNDO
			nodes.clear();
			lines.clear();
			selected_nodes.clear();
			dragging_nodes.clear(); // TODO: may cause bug where if you press z while dragging node something bad happens
			fractal_stack.pop_back();
			for (const auto& unused : fractal_stack.back().getDerivedLines()) {
				notifyAll(Event::FRACTAL_CHANGED);
			}*/
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (isWithinEditingFrame({ event.mouseButton.x, event.mouseButton.y })) {

			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				left_press_location = mouse_framepos;
				Vec2 transformed_pos = Vec2(event.mouseButton.x, event.mouseButton.y) - global_offset;

				// case 1: The user has clicked down on selected node(s).
				// They are added to the set of nodes being dragged, and the unselected nodes are skipped.
				bool on_any_selnode = false;
				for (int node_id : frame->getSelectedNodes()) {
					const EditableLineNode* node = frame->getNodes().at(node_id);
					if (node->pointIntersection(transformed_pos)) {
						Vec2 selected_offset = transformed_pos - node->getPosition();
						dragging_nodes.emplace(node_id, selected_offset);
						on_any_selnode = true;
					}
				}

				// case 2: No selected nodes were clicked down on.
				// if any non selected nodes are clicked down on, they are added to the set of dragging nodes.
				if (!on_any_selnode) {
					for (const auto& id_node : frame->getNodes()) {
						if (id_node.second->pointIntersection(transformed_pos)) {
							Vec2 selected_offset = transformed_pos - id_node.second->getPosition();
							dragging_nodes.emplace(id_node.first, selected_offset);
						}
					}
				}
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (isWithinEditingFrame({ event.mouseMove.x, event.mouseMove.y })) {
			Vec2 transformed_pos = Vec2(event.mouseMove.x, event.mouseMove.y) - global_offset;

			// if dragging nodes exist move them by the correct amount
			bool node_moved = false;
			for (auto& pair : dragging_nodes) {
				node_moved = true;
				Vec2 translation = transformed_pos - (frame->getNodes().at(pair.first)->getPosition() + pair.second);
				moveNode(pair.first, translation);
			}
			if (node_moved) {
				remapFractal();
				notifyAll(Event::LINES_CHANGED);
			}
			mouse_framepos.x = event.mouseMove.x;
			mouse_framepos.y = event.mouseMove.y;
			notifyAll(Event::MOUSE_MOVED);
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (isWithinEditingFrame({ event.mouseButton.x, event.mouseButton.y })) {
			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				dragging_nodes.clear();
				if (mouse_framepos == left_press_location) {
					frame->clearSelection();

					// select new nodes
					for (auto& node : frame->getNodes()) {
						if (node.second->pointIntersection(mouse_framepos - global_offset)) {
							frame->selectNode(node.first);
						}
					}
					
					if (DEBUG) {
						PRINTLN("Seletec ids:");
						for (int i : frame->getSelectedNodes()) {
							PRINTLN(i);
						}
						PRINTLN("");
					}
					notifyAll(Event::SELECTION_CHANGED);
				}
			}
		}
	}
}



void Editing::recalcEditingFrameDimensions(Vec2 window_dimensions) {
	editing_frame_size.x = (window_dimensions.x - right_panel_width);
	editing_frame_size.y = window_dimensions.y;
	global_offset = editing_frame_size / 2;
}

Vec2 Editing::getMousePosInFrame() const
{
	return mouse_framepos;
}

double* Editing::getValClipboard()
{
	return &value_clipboard;
}


void Editing::setHoveredNode(int node_id)
{
	assert((frame->getSelectedNodes().count(node_id) == 1 || node_id == -1) && "hovering over a non-selected node (or duplicate id)");
	hovered_node = node_id;
	notifyAll(Event::HOVERED_NODE_CHANGED);
}

int Editing::getHoveredNode() const
{
	return hovered_node;
}

void Editing::clearHoveredNode()
{
	hovered_node = -1;
	notifyAll(Event::HOVERED_NODE_CHANGED);
}

void Editing::selectOnlyHoveredNode()
{
	frame->clearSelection();
	frame->selectNode(hovered_node);
	notifyAll(Event::SELECTION_CHANGED);
}

bool Editing::nodeIsHovered() const
{
	if (hovered_node == -1) {
		return false;
	}
	return true;
}

void Editing::newLine() {
	float length = rand() % 50 + 50; // random length from 50 - 100
	float angle = ((float)(rand() % (3141 * 2))) / 100; // random angle in radians from 0 - ~2pi
	Vec2 position = Vec2::fromLenDir(length, angle);
	addLine({ -position, position });
}

void Editing::addLine(AbsLine l)
{
	frame->addLine(l);
	remapFractal();
	notifyAll(Event::LINES_CHANGED);
}

void Editing::removeSelectedLines()
{
	const std::unordered_map<int, EditableLineNode*>& nodes = frame->getNodes();
	for (int node_id : frame->getSelectedNodes()) {
		if (nodes.count(node_id)) {
			const EditableLineNode* node = nodes.at(node_id);
			int line_id = node->getLine()->getID();
			frame->removeLine(line_id);
		}
	}
	frame->clearSelection();
	clearHoveredNode();
	notifyAll(Event::SELECTION_CHANGED);
	notifyAll(Event::LINES_CHANGED);
	remapFractal();
}

void Editing::setNodePosition(int node_id, Vec2 new_pos)
{
	const EditableLineNode* node = frame->getNodes().at(node_id);
	if (nodeTranslationLegal(*node, new_pos)) {
		frame->setNodePos(node->getID(), new_pos);
		remapFractal();
		notifyAll(Event::LINES_CHANGED);
	}
}

int Editing::getNumRecursions() const
{
	return num_recursions;
}

void Editing::setNumRecursions(int num)
{
	num_recursions = num;
}

Vec2 Editing::getEditingFrameSize() const
{
	return editing_frame_size;
}

void Editing::setNodeAngle(int node_id, double angle)
{
	const EditableLineNode* node = frame->getNodes().at(node_id);
	Vec2 new_pos = node->getPosition() + Vec2::fromLenDir(node->getLength(), angle);
	setNodePosition(node->getOtherNode()->getID(), new_pos);
}

void Editing::setNodeLength(int node_id, double length)
{
	const EditableLineNode* node = frame->getNodes().at(node_id);
	Vec2 new_pos = node->getPosition() + Vec2::fromLenDir(length, node->getAngle());
	setNodePosition(node->getOtherNode()->getID(), new_pos);
}

void Editing::remapFractal()
{
	AbsLine base_line = frame->getBaseLine().toAbsLine();
	fractalGen.setBaseLine(base_line);
	std::vector<LFLine> final_lines;
	for (const auto& line : frame->getLines()) {
		LFLine lfl = line.second.toLFLine(base_line);
		final_lines.push_back(lfl);
	}
	fractalGen.setDerivedLines(final_lines);
	frame->setFractal(fractalGen.generate());
	notifyAll(Event::FRACTAL_CHANGED);
}

void Editing::moveNode(int node_id, Vec2 translation)
{
	const EditableLineNode* node = frame->getNodes().at(node_id);
	Vec2 new_pos = node->getPosition() + translation;
	const EditableLineNode* other = node->getOtherNode();
	AbsLine new_line = { other->getPosition(), new_pos };
	double max_line_length = lineLength(frame->getBaseLine().toAbsLine()) - MAX_LINE_LEN_LEWAY;
	if (new_line.length() > max_line_length) {
		new_pos = other->getPosition() + Vec2::fromLenDir(max_line_length, lineAngle(new_line));
		const Vec2 mouse_framepos_offset = mouse_framepos - node->getPosition();
		mouse_framepos = node->getPosition() + mouse_framepos_offset;
	}
	frame->setNodePos(node->getID(), new_pos);
}

bool Editing::isWithinEditingFrame(const Vec2& point) const
{
	if (point.x >= 0 && point.y >= 0 && point.x < editing_frame_size.x && point.y < editing_frame_size.y) {
		return true;
	}
	return false;
}

bool Editing::nodeTranslationLegal(const EditableLineNode& node, const Vec2& new_pos) const
{
	const EditableLineNode* other = node.getOtherNode();
	AbsLine new_line = { other->getPosition(), new_pos };
	double max_line_length = lineLength(frame->getBaseLine().toAbsLine()) - MAX_LINE_LEN_LEWAY;
	return (new_line.length() <= max_line_length);
}
