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
	recalcEditingFrameCenter(state->getRenderWindow()->getSize().x, state->getRenderWindow()->getSize().y);
	AbsLine l = { editing_frame_center.x-200, editing_frame_center.y, editing_frame_center.x + 200, editing_frame_center.y };
	base_line = std::make_shared<EditableLine>(getID(), getID(), getID(), l);
	fractal->setBaseLine(l);
	fractal->generate(num_recursions);
}

void Editing::handleEvent(sf::Event& event)
{
	if (event.type == sf::Event::Resized) {
		recalcEditingFrameCenter(event.size.width, event.size.height);
	}
	else if (event.type == sf::Event::MouseButtonPressed) {
		if (isWithinEditingFrame(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				left_press_location = mouse_framepos;
				for (auto& node : nodes) {
					if (node.second->pointIntersection(event.mouseButton.x, event.mouseButton.y)) {
						dragging_nodes.emplace(node.first);
					}
				}
			}
		}
	}
	else if (event.type == sf::Event::MouseMoved) {
		if (isWithinEditingFrame(sf::Vector2f(event.mouseMove.x, event.mouseMove.y))) {		
			bool node_moved = false;
			for (int node_id : dragging_nodes) {
				node_moved = true;
				nodes[node_id]->translate(event.mouseMove.x - mouse_framepos.x, event.mouseMove.y - mouse_framepos.y);
			}
			if (node_moved) {
				if (DEBUG) {
					int i = 0;
					for (auto l : lines) {
						LFLine lfl = l.second->toLFLine(base_line->toAbsLine());
						PRINT(i << ": " << lfl.distance << " | " << lfl.angle1 << " | " << lfl.length << " | " << lfl.angle2);
						i++;
					}
				}
				notifyAll(Event::LINES_CHANGED);
				fractalChanged();
			}
			mouse_framepos.x = event.mouseMove.x;
			mouse_framepos.y = event.mouseMove.y;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased) {
		if (isWithinEditingFrame(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
			if (event.mouseButton.button == sf::Mouse::Button::Left) {
				dragging_nodes.clear();
				if (mouse_framepos == left_press_location) {
					selected_nodes.clear();

					// select new nodes
					for (auto& node : nodes) {
						if (node.second->pointIntersection(mouse_framepos.x, mouse_framepos.y)) {
							selected_nodes.insert(node.first);
						}
					}
					if (DEBUG) {
						PRINT("Seletec ids:");
						for (int i : selected_nodes) {
							PRINT(i);
						}
						PRINT("");
					}
					notifyAll(Event::SELECTION_CHANGED);
				}
			}
		}
	}
}

void Editing::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(fractal->getFractal());
}

void Editing::recalcEditingFrameCenter(int window_width, int window_height) {
	editing_frame_size.x = (window_width - right_panel_width);
	editing_frame_size.y = window_height;
	editing_frame_center.x = editing_frame_size.x / 2;
	editing_frame_center.y = editing_frame_size.y / 2;
}

sf::Vector2i Editing::getEditingFrameCenter() const
{
	return editing_frame_center;
}

sf::Vector2i Editing::getMousePosInFrame() const
{
	return mouse_framepos;
}

void Editing::addLine() {
	float length = rand() % 50 + 50; // random length from 50 - 100
	float angle = ((float)(rand() % (3141 * 2))) / 100; // random angle in radians from 0 - ~2pi
	float x = lendirX(length, angle);
	float y = lendirY(length, angle);

	int id_arr[] = { getID(), getID(), getID() };
	AbsLine l = { editing_frame_center.x - x, editing_frame_center.y - y, editing_frame_center.x + x, editing_frame_center.y + y };
	std::shared_ptr<EditableLine> line = std::make_shared<EditableLine>(id_arr[0], id_arr[1], id_arr[2], l);

	lines.emplace(id_arr[0], line);
	nodes.emplace(id_arr[1], line->getNodeA());
	nodes.emplace(id_arr[2], line->getNodeB());
	notifyAll(Event::LINES_CHANGED);
}

const std::unordered_map<int, std::shared_ptr<EditableLineNode>>& Editing::getNodes() const
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

int Editing::getNumRecursions() const
{
	return num_recursions;
}

void Editing::setNumRecursions(int num)
{
	num_recursions = num;
}

sf::Vector2i Editing::getEditingFrameSize() const
{
	return editing_frame_size;
}

void Editing::addObserver(Observer* observer)
{
	observers.insert(observer);
}

void Editing::fractalChanged()
{
	std::vector<LFLine> final_lines;
	for (auto& line : lines) {
		LFLine lfl = line.second->toLFLine(base_line->toAbsLine());
		final_lines.push_back(lfl);
	}
	fractal->setDerivedLines(final_lines);
	fractal->generate(num_recursions);
}

void Editing::notifyAll(Event e) const
{
	for (Observer* observer : observers) {
		observer->onNotify(e);
	}
}

bool Editing::isWithinEditingFrame(sf::Vector2f point) const
{
	if (point.x >= 0 && point.y >= 0 && point.x < editing_frame_size.x && point.y < editing_frame_size.y) {
		return true;
	}
	return false;
}
