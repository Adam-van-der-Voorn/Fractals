#include "SelLineWidget.h"
#include "EditableLineNode.h"
#include "Editing.h"
#include <unordered_map>

class Editing;

SelLineWidget::SelLineWidget(Editing* editing, int node_id, float width, float height, float xpos, float ypos)
	: node_id(node_id)
{
	panel->setPosition(xpos, ypos);
	panel->setSize(width, height);
	panel->setWidgetName(std::to_string(node_id));
	panel->add(select_button);
	select_button->setSize("100%", "100%");
	select_button->onClick(&Editing::selectOnlyHoveredNode, editing);
	select_button->onMouseEnter(&Editing::setHoveredNode, editing, node_id);
    select_button->onMouseLeave(&Editing::setHoveredNode, editing, -1);
}

tgui::Panel::Ptr SelLineWidget::getPanel()
{
	return panel;
}