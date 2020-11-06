#include "SelLineWidget.h"
#include "EditableLineNode.h"
#include <unordered_map>
void highlight(std::unordered_map<int, std::shared_ptr<EditableLineNode>>* nodes, int node_id) {
	//(*nodes)[node_id]->getDrNode().setOutlineColor(sf::Color::Green);
}

void unhighlight(std::unordered_map<int, std::shared_ptr<EditableLineNode>>* nodes, int node_id)
{
	//(*nodes)[node_id]->getDrNode().setOutlineColor(sf::Color::Red);
}

void select(int node_id)
{

}
SelLineWidget::SelLineWidget(std::unordered_map<int, std::shared_ptr<EditableLineNode>>& nodes, std::unordered_set<int>& selected_nodes,
	int node_id, float width, float height, float xpos, float ypos)
	: nodes(nodes), selected_nodes(selected_nodes), node_id(node_id)
{
	panel->setPosition(xpos, ypos);
	panel->setSize(width, height);
	panel->add(select_button);
	select_button->setSize("100%", "100%");
	select_button->onMouseEnter(highlight, &nodes, node_id);
	select_button->onMouseLeave(unhighlight, &nodes, node_id);
}

tgui::Panel::Ptr SelLineWidget::getPanel()
{
	return panel;
}