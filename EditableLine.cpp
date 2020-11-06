#include "EditableLine.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "vecutil.h"
#include "LFLine.h"
#include "EditableLineNode.h"
#include <memory>
#include <cassert>

EditableLine::EditableLine(int line_id, int node_id_1, int node_id_2, AbsLine line) : id(line_id)
{
	a = std::make_shared<EditableLineNode>(node_id_1, line.x1, line.y1, this);
	b = std::make_shared<EditableLineNode>(node_id_2, line.x2, line.y2, this);
}

std::shared_ptr<EditableLineNode> EditableLine::getNodeA() const
{
	return a;
}

std::shared_ptr<EditableLineNode> EditableLine::getNodeB() const
{
	return b;
}

void EditableLine::setRecursive(bool b)
{
	recursive = b;
}

LFLine EditableLine::toLFLine(AbsLine base_line) const
{
	double base_line_len = lineLength(base_line);
	double base_line_angle = lineAngle(base_line);
	double distance = distanceBetweenAB(base_line.x1, base_line.y1, a->getX(), a->getY()) / base_line_len;
	double angle1 = angleBetweenAB(base_line.x1, base_line.y1, a->getX(), a->getY()) - base_line_angle;
	double length = distanceBetweenAB(a->getX(), a->getY(), b->getX(), b->getY()) / base_line_len;
	double angle2 = angleBetweenAB(a->getX(), a->getY(), b->getX(), b->getY()) - base_line_angle;
	return { distance, angle1, length, angle2, recursive};
}

AbsLine EditableLine::toAbsLine() const
{
	return { a->getX(), a->getY(), b->getX(), b->getY() };
}

