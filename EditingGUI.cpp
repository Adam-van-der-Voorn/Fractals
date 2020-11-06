#include "EditingGUI.h"
#include "Editing.h"
#include "EditableLine.h"
#include "EditableLineNode.h"
#include "debug_printing.h"
#include <unordered_set>
#include <unordered_map>
#include <memory>

EditingGUI::EditingGUI(Editing * editing) : editing(editing)
{
	auto a = baseLine[0].position;
	auto b = a.x;
	auto z = editing->getBaseLine();
	auto v = z->getNodeA();
	auto n = v->getX();
	baseLine[0].position.x = editing->getBaseLine()->getNodeA()->getX();
	baseLine[0].position.y = editing->getBaseLine()->getNodeA()->getY();
	baseLine[0].color = sf::Color::Color(80, 80, 80);

	baseLine[1].position.x = editing->getBaseLine()->getNodeB()->getX();
	baseLine[1].position.y = editing->getBaseLine()->getNodeB()->getY();
	baseLine[1].color = sf::Color::Color(80, 80, 80);

	editing->addObserver(this);
}

void EditingGUI::onNotify(int event_num)
{
	auto event = static_cast<Editing::Event>(event_num);
	switch (event)
	{
	case Editing::LINES_CHANGED:
		// make data structs the correct size
		assert(editing->getNodes().size() % 2 == 0 && "uneven amount of nodes");
		nodeLines.resize(editing->getNodes().size()); // using num of nodes instead of num of lines because each line has two verts
		nodes.resize(editing->getNodes().size());
		updateLines();
		updateNodes();
		break;
	case Editing::SELECTION_CHANGED:
		assert(editing->getNodes().size() % 2 == 0 && "uneven amount of nodes");
		nodes.resize(editing->getNodes().size());
		updateNodes();
		break;
	default:
		assert(false && "bad enum somehow");
		break;
	}
}

void EditingGUI::updateNodes()
{
	int i = 0;
	for (const auto& node_pair : editing->getNodes()) {
		nodes[i].setPosition(sf::Vector2f(node_pair.second->getX(), node_pair.second->getY()));
		nodes[i].setRadius(EditableLineNode::NODE_RADIUS);
		nodes[i].setFillColor(sf::Color::Transparent);
		nodes[i].setOutlineColor(sf::Color::White);
		nodes[i].setOutlineThickness(1.0f);
		nodes[i].setOrigin(sf::Vector2f(EditableLineNode::NODE_RADIUS, EditableLineNode::NODE_RADIUS));
		if (editing->getSelectedNodes().count(node_pair.first)) { // node is selected
			nodes[i].setOutlineColor(sf::Color::Red);
		}
		else {
			nodes[i].setOutlineColor(sf::Color::White);
		}
		i++;
	}
}

void EditingGUI::updateLines()
{
	int i = 0;
	for (const auto& line_pair : editing->getLines()) {
		std::shared_ptr<EditableLineNode> node_a = line_pair.second->getNodeA();
		nodeLines[i].position.x = node_a->getX();
		nodeLines[i].position.y = node_a->getY();
		i++;
		std::shared_ptr<EditableLineNode> node_b = line_pair.second->getNodeB();
		nodeLines[i].position.x = node_b->getX();
		nodeLines[i].position.y = node_b->getY();
		i++;
	}
}

void EditingGUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(nodeLines);
	for (auto& node : nodes) {
		target.draw(node);
	}
	target.draw(baseLine);
}
