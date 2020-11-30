#include "EditableLine.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "vecutil.h"
#include "LFLine.h"
#include "EditableLineNode.h"
#include <memory>
#include <cassert>

EditableLine::EditableLine(int line_id, int back_node_id, int front_node_id, AbsLine line) : id(line_id)
{
	// have to do temp variables for some reason, cannot just init in the func call
	Vec2 tmp_back = { line.back_x, line.back_y }, tmp_front = { line.head_x, line.head_y };
	back_node = std::make_shared<EditableLineNode>(back_node_id, tmp_back, false, this);
	front_node = std::make_shared<EditableLineNode>(front_node_id, tmp_front, true, this);
}

std::shared_ptr<EditableLineNode> EditableLine::getBackNode() const
{
	return back_node;
}

std::shared_ptr<EditableLineNode> EditableLine::getFrontNode() const
{
	return front_node;
}

void EditableLine::setRecursive(bool front_node)
{
	recursive = front_node;
}

LFLine EditableLine::toLFLine(AbsLine base_line) const
{
	double base_line_len = lineLength(base_line);
	double base_line_angle = lineAngle(base_line);
	double distance = distanceBetweenAB(base_line.back_x, base_line.back_y, back_node->getPos().x, back_node->getPos().y) / base_line_len;
	double angle1 = angleBetweenAB(base_line.back_x, base_line.back_y, back_node->getPos().x, back_node->getPos().y) - base_line_angle;
	double length = distanceBetweenAB(back_node->getPos().x, back_node->getPos().y, front_node->getPos().x, front_node->getPos().y) / base_line_len;
	double angle2 = angleBetweenAB(back_node->getPos().x, back_node->getPos().y, front_node->getPos().x, front_node->getPos().y) - base_line_angle;
	return { distance, angle1, length, angle2, recursive};
}

AbsLine EditableLine::toAbsLine() const
{
	return { back_node->getPos().x, back_node->getPos().y, front_node->getPos().x, front_node->getPos().y };
}

int EditableLine::getID() const
{
	return id;
}

