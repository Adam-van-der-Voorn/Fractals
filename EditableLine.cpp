#include "EditableLine.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "vecutil.h"
#include "LFLine.h"
#include "EditableLineNode.h"
#include <memory>
#include <cassert>

EditableLine::EditableLine(int line_id, int back_node_id, int front_node_id, AbsLine line) : id(line_id)
{
	back_node = std::make_shared<EditableLineNode>(back_node_id, line.back_x, line.back_y, false, this);
	front_node = std::make_shared<EditableLineNode>(front_node_id, line.head_x, line.head_y, true, this);
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
	double distance = distanceBetweenAB(base_line.back_x, base_line.back_y, back_node->getX(), back_node->getY()) / base_line_len;
	double angle1 = angleBetweenAB(base_line.back_x, base_line.back_y, back_node->getX(), back_node->getY()) - base_line_angle;
	double length = distanceBetweenAB(back_node->getX(), back_node->getY(), front_node->getX(), front_node->getY()) / base_line_len;
	double angle2 = angleBetweenAB(back_node->getX(), back_node->getY(), front_node->getX(), front_node->getY()) - base_line_angle;
	return { distance, angle1, length, angle2, recursive};
}

AbsLine EditableLine::toAbsLine() const
{
	return { back_node->getX(), back_node->getY(), front_node->getX(), front_node->getY() };
}

int EditableLine::getID() const
{
	return id;
}

