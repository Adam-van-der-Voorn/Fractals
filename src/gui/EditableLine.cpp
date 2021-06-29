#include "EditableLine.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "vecutil.h"
#include "LFLine.h"
#include "EditableLineNode.h"
#include <memory>
#include <cassert>

// single line obj 
// 
// abs line
// back node
// 

EditableLine::EditableLine(int line_id, int back_node_id, int front_node_id, const AbsLine& line) :
id(line_id)
{
	// have to do temp variables for some reason, cannot just init in the func call
	Vec2 tmp_back = { line.back.x, line.back.y }, tmp_front = { line.head.x, line.head.y };
	back_node = std::make_unique<EditableLineNode>(back_node_id, tmp_back, false, this);
	front_node = std::make_unique<EditableLineNode>(front_node_id, tmp_front, true, this);
}

EditableLineNode* EditableLine::backNode()
{
	return back_node.get();
}

EditableLineNode* EditableLine::frontNode()
{
	return front_node.get();
}

const EditableLineNode* EditableLine::getBackNode() const
{
	return back_node.get();
}

const EditableLineNode* EditableLine::getFrontNode() const
{
	return front_node.get();
}

void EditableLine::setRecursive(bool front_node)
{
	recursive = front_node;
}

bool EditableLine::isRecursive() const
{
	return recursive;
}

LFLine EditableLine::toLFLine(AbsLine base_line) const
{
	double base_line_len = lineLength(base_line);
	double base_line_angle = lineAngle(base_line);
	double distance = distanceBetweenAB(base_line.back.x, base_line.back.y, back_node->getPosition().x, back_node->getPosition().y) / base_line_len;
	double angle1 = angleBetweenAB(base_line.back.x, base_line.back.y, back_node->getPosition().x, back_node->getPosition().y) - base_line_angle;
	double length = distanceBetweenAB(back_node->getPosition().x, back_node->getPosition().y, front_node->getPosition().x, front_node->getPosition().y) / base_line_len;
	double angle2 = angleBetweenAB(back_node->getPosition().x, back_node->getPosition().y, front_node->getPosition().x, front_node->getPosition().y) - base_line_angle;
	return { distance, angle1, length, angle2, recursive};
}

AbsLine EditableLine::toAbsLine() const
{
	return { back_node->getPosition(), front_node->getPosition() };
}

int EditableLine::getID() const
{
	return id;
}

