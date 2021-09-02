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
#include "NodeID.h"
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
	Vec2 half_frame = editing_frame_size/2;
	fractalGen.setView(RightAngleRect(-half_frame, half_frame));
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
				for (const NodeID& node_id : frame->getSelectedNodes()) {
					const EditableLineNode* node = frame->getNode(node_id);
					if (node->pointIntersection(transformed_pos)) {
						Vec2 selected_offset = transformed_pos - node->getPosition();
						dragging_nodes.emplace(node_id, selected_offset);
						on_any_selnode = true;
					}
				}

				// case 2: No selected nodes were clicked down on.
				// if any non selected nodes are clicked down on, they are added to the set of dragging nodes.
				if (!on_any_selnode) {
					for (const std::pair<NodeID, const EditableLineNode*>& node_pair : frame->getNodes()) {
						if (node_pair.second->pointIntersection(transformed_pos)) {
							Vec2 selected_offset = transformed_pos - node_pair.second->getPosition();
							dragging_nodes.emplace(node_pair.first, selected_offset);
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
				Vec2 translation = transformed_pos - (frame->getNode(pair.first)->getPosition() + pair.second);
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


void Editing::setHoveredNode(NodeID node_id)
{
	assert((frame->getSelectedNodes().count(node_id) == 1 || node_id.lineID() == -1) && "hovering over a non-selected node (or duplicate id)");
	hovered_node = node_id;
	notifyAll(Event::HOVERED_NODE_CHANGED);
}

NodeID Editing::getHoveredNode() const
{
	return hovered_node;
}

void Editing::clearHoveredNode()
{
	hovered_node = NodeID::nonexistent();
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
	if (hovered_node == NodeID::nonexistent()) {
		return false;
	}
	return true;
}

void Editing::newLine() {
	float length = rand() % 200 + 100; // random length from 100 - 200
	float angle = ((float)(rand() % (3141 * 2))) / 100; // random angle in radians from 0 - ~2pi
	float offset_len = rand() % 20 + 5; // random offset from 5 - 25
	float offset_angle = ((float)(rand() % (3141 * 2))) / 100;
	Vec2 offset = Vec2::fromLenDir(offset_len, offset_angle);
	Vec2 vec = Vec2::fromLenDir(length, angle)/2;
	addLine({ -vec + offset, vec + offset });
}

void Editing::addLine(AbsLine l)
{
	frame->addLine(l);
	remapFractal();
	notifyAll(Event::LINES_CHANGED);
}

void Editing::removeSelectedLines()
{
	const std::unordered_map<int, EditableLine>& lines = frame->getLines();
	for (NodeID selected : frame->getSelectedNodes()) {
		assert(lines.count(selected.lineID())); // assert line exists with the selection id
		frame->removeLine(selected.lineID());
	}
	frame->clearSelection();
	clearHoveredNode();
	notifyAll(Event::SELECTION_CHANGED);
	notifyAll(Event::LINES_CHANGED);
	remapFractal();
}

void Editing::setNodePosition(NodeID node_id, Vec2 new_pos)
{
	const EditableLineNode* node = frame->getNode(node_id);
	if (nodeTranslationLegal(*node, new_pos)) {
		frame->setNodePos(node_id, new_pos);
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

void Editing::setNodeAngle(NodeID node_id, double angle)
{
	const EditableLine& line = frame->getLine(node_id.lineID());
	const EditableLineNode* node = frame->getNode(node_id);
	Vec2 new_pos = node->getPosition() + Vec2::fromLenDir(line.getLength(), angle);
	NodeID other_node_id = NodeID(line.getID(), !node->isFront());
	setNodePosition(other_node_id, new_pos);
}

void Editing::setNodeLength(NodeID node_id, double length)
{
	const EditableLine& line = frame->getLine(node_id.lineID());
	const EditableLineNode* node = frame->getNode(node_id);
	Vec2 new_pos = node->getPosition() + Vec2::fromLenDir(length, line.getAngle(node_id.isFront()));
	NodeID other_node_id = NodeID(line.getID(), !node->isFront());
	setNodePosition(other_node_id, new_pos);
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

void Editing::moveNode(NodeID node_id, Vec2 translation)
{

	const EditableLineNode* node = frame->getNode(node_id);
	Vec2 new_pos = node->getPosition() + translation;
	const EditableLineNode* other = frame->getNode(NodeID::other(node_id));

	// the dimensions of the line after the node has been moved
	AbsLine new_line = { other->getPosition(), new_pos };

	// check if the new line fits length limit. If not, adjust.
	double max_line_length = lineLength(frame->getBaseLine().toAbsLine()) - MAX_LINE_LEN_LEWAY;
	if (new_line.length() > max_line_length) {
		new_pos = other->getPosition() + Vec2::fromLenDir(max_line_length, lineAngle(new_line));
		const Vec2 mouse_framepos_offset = mouse_framepos - node->getPosition();
		mouse_framepos = node->getPosition() + mouse_framepos_offset;
	}
	frame->setNodePos(node_id, new_pos);
}

bool Editing::isWithinEditingFrame(const Vec2& point) const
{
	if (point.x >= 0 && point.y >= 0 && point.x < editing_frame_size.x && point.y < editing_frame_size.y) {
		return true;
	}
	return false;
}

bool Editing::nodeTranslationLegal(const EditableLineNode& fixed_node, const Vec2& new_pos) const
{
	AbsLine new_line = { fixed_node.getPosition(), new_pos };
	double max_line_length = lineLength(frame->getBaseLine().toAbsLine()) - MAX_LINE_LEN_LEWAY;
	return (new_line.length() <= max_line_length);
}
