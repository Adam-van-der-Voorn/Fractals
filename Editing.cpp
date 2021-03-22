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
#include <cstdlib>
#include <string>
#include <memory>

int getID() {
	static int i = 0;
	return i++;
}

Editing::Editing(EditingState* state, LineFractal* fractal) :
	state(state), fractal(fractal)
{
	// set seed for line creation
	srand(80085);

	// setup base line
	recalcEditingFrameDimensions(vec2FromSF(state->getRenderWindow()->getSize()));
	AbsLine l = { {-200, 0}, {+200, 0} };
	base_line = std::make_shared<EditableLine>(getID(), getID(), getID(), l);
	fractal->setBaseLine(l);
	fractal->generateIter(num_recursions);

	//setup hovered node
	clearHoveredNode();
}

void Editing::setLineRecursiveness(int line_id, bool b)
{
	lines[line_id]->setRecursive(b);
	notifyAll(Event::LINES_CHANGED);
	updateFractal();
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
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (isWithinEditingFrame({ event.mouseButton.x, event.mouseButton.y })) {
			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				left_press_location = mouse_framepos;
				Vec2 transformed_pos = Vec2(event.mouseButton.x, event.mouseButton.y) - global_offset;
				bool on_any_selnode = false;
				for (int node_id : selected_nodes) {
					EditableLineNode* node = nodes[node_id];
					if (node->pointIntersection(transformed_pos)) {
						Vec2 selected_offset = transformed_pos - node->getPosition();
						dragging_nodes.emplace(node_id, selected_offset);
						on_any_selnode = true;
					}
				}
				if (!on_any_selnode) {
					for (auto& node : nodes) {
						if (node.second->pointIntersection(transformed_pos)) {
							Vec2 selected_offset = transformed_pos - node.second->getPosition();
							dragging_nodes.emplace(node.first, selected_offset);
						}
					}
				}
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (isWithinEditingFrame({ event.mouseMove.x, event.mouseMove.y })) {
			Vec2 transformed_pos = Vec2(event.mouseMove.x, event.mouseMove.y) - global_offset;

			bool node_moved = false;
			for (auto& pair : dragging_nodes) {
				node_moved = true;
				Vec2 translation = transformed_pos - (nodes[pair.first]->getPosition() + pair.second);
				moveNode(pair.first, translation);
			}
			if (node_moved) {
				notifyAll(Event::LINES_CHANGED);
				updateFractal();
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
					selected_nodes.clear();

					// select new nodes
					for (auto& node : nodes) {
						if (node.second->pointIntersection(mouse_framepos - global_offset)) {
							selected_nodes.insert(node.first);
						}
					}
					
					if (DEBUG) {
						PRINTLN("Seletec ids:");
						for (int i : selected_nodes) {
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
	assert((selected_nodes.count(node_id) == 1 || node_id == -1) && "hovering over a non-selected node (or duplicate id)");
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
	selected_nodes.clear();
	selected_nodes.insert(hovered_node);
	notifyAll(Event::SELECTION_CHANGED);
}

bool Editing::nodeIsHovered() const
{
	if (hovered_node == -1) {
		return false;
	}
	return true;
}

void Editing::addLine() {
	float length = rand() % 50 + 50; // random length from 50 - 100
	float angle = ((float)(rand() % (3141 * 2))) / 100; // random angle in radians from 0 - ~2pi
	Vec2 position = Vec2::fromLenDir(length, angle);

	int id_arr[] = { getID(), getID(), getID() };
	AbsLine l = {Vec2(0,0)-position, position };
	std::shared_ptr<EditableLine> line = std::make_shared<EditableLine>(id_arr[0], id_arr[1], id_arr[2], l);

	lines.emplace(id_arr[0], line);
	nodes.emplace(id_arr[1], line->getBackNode());
	nodes.emplace(id_arr[2], line->getFrontNode());
	updateFractal();
	notifyAll(Event::LINES_CHANGED);
}

void Editing::removeSelectedLines()
{
	for (int node_id : selected_nodes) {
		if (nodes.count(node_id)) {
			EditableLine* line = nodes[node_id]->getLine();
			nodes.erase(line->getFrontNode()->getID());
			nodes.erase(line->getBackNode()->getID());
			lines.erase(line->getID());
		}
		
	}
	selected_nodes.clear();
	clearHoveredNode();
	notifyAll(Event::SELECTION_CHANGED);
	notifyAll(Event::LINES_CHANGED);
	updateFractal();
}

void Editing::setNodePosition(int node_id, Vec2 new_pos)
{
	EditableLineNode* node = nodes[node_id];
	if (nodeTranslationLegal(node, new_pos)) {
		node->setPosition(new_pos);
	}
	notifyAll(Event::LINES_CHANGED);
	updateFractal();
}

const std::unordered_map<int, EditableLineNode*>& Editing::getNodes() const
{
	return nodes;
}

const std::unordered_map<int, std::shared_ptr<EditableLine>>& Editing::getLines() const
{
	return lines;
}

const std::shared_ptr<EditableLine> Editing::getBaseLine() const
{
	return base_line;
}

const std::unordered_set<int>& Editing::getSelectedNodes() const
{
	return selected_nodes;
}

const LineFractal* Editing::getFractal() const
{
	return fractal;
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
	EditableLineNode* node = nodes[node_id];
	Vec2 new_pos = node->getPosition() + Vec2::fromLenDir(node->getLength(), angle);
	if (nodeTranslationLegal(node->getOtherNode(), new_pos)) {
		node->getOtherNode()->setPosition(new_pos);
		notifyAll(Event::LINES_CHANGED);
		updateFractal();
	}
}

void Editing::setNodeLength(int node_id, double length)
{
	EditableLineNode* node = nodes[node_id];
	Vec2 new_pos = node->getPosition() + Vec2::fromLenDir(length, node->getAngle());
	if (nodeTranslationLegal(node->getOtherNode(), new_pos)) {
		node->getOtherNode()->setPosition(new_pos);
		notifyAll(Event::LINES_CHANGED);
		updateFractal();
	}
}

void Editing::updateFractal()
{
	std::vector<LFLine> final_lines;
	for (auto& line : lines) {
		LFLine lfl = line.second->toLFLine(base_line->toAbsLine());
		final_lines.push_back(lfl);
	}
	fractal->setDerivedLines(final_lines);
	fractal->generateIter(num_recursions);
	notifyAll(Event::FRACTAL_CHANGED);
}

void Editing::moveNode(int node_id, Vec2 translation)
{
	EditableLineNode* node = nodes[node_id];
	Vec2 new_pos = node->getPosition() + translation;
	EditableLineNode* other = nodes[node_id]->getOtherNode();
	AbsLine new_line = { other->getPosition(), new_pos };
	double max_line_length = lineLength(base_line->toAbsLine()) - MAX_LINE_LEN_LEWAY;
	if (new_line.length() > max_line_length) {
		const Vec2 mouse_framepos_offset = mouse_framepos - node->getPosition();
		new_pos = other->getPosition() + Vec2::fromLenDir(max_line_length, lineAngle(new_line));
		node->setPosition(new_pos);
		mouse_framepos = node->getPosition() + mouse_framepos_offset;
	}
	else {
		node->translate(translation);
	}
}

bool Editing::isWithinEditingFrame(Vec2 point) const
{
	if (point.x >= 0 && point.y >= 0 && point.x < editing_frame_size.x && point.y < editing_frame_size.y) {
		return true;
	}
	return false;
}

bool Editing::nodeTranslationLegal(EditableLineNode* node, const Vec2& new_pos)
{
	EditableLineNode* other = node->getOtherNode();
	AbsLine new_line = { other->getPosition(), new_pos };
	double max_line_length = lineLength(base_line->toAbsLine()) - MAX_LINE_LEN_LEWAY;
	return (new_line.length() <= max_line_length);
}
