#include "EditableLineNode.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "LFLine.h"
#include <cassert>

EditableLineNode::EditableLineNode(int id, Vec2 pos, bool is_front, EditableLine* line) : id(id), pos(pos), is_front(is_front), line(line)
{
}

bool EditableLineNode::pointIntersection(Vec2 point) const
{
	if (distanceBetweenAB(point.x, point.y, pos.x, pos.y) <= NODE_RADIUS) {
		return true;
	}
	return false;
}

void EditableLineNode::translate(Vec2 translation)
{
	pos += translation;
}

void EditableLineNode::setPosition(Vec2 position)
{
	pos = position;
}

void EditableLineNode::setLength(double length)
{
	assert(false && "func not defined");
}

void EditableLineNode::setAngle(double angle)
{
	assert(false && "func not defined");
}

void EditableLineNode::setRecursive(bool front_node)
{
	assert(false && "idk if this should be here");
}

Vec2 EditableLineNode::getPos() const
{
	return pos;
}

bool EditableLineNode::isFront() const
{
	return is_front;
}
EditableLine* EditableLineNode::getLine() const
{
	return line;
}
std::shared_ptr<EditableLineNode> EditableLineNode::getOtherNode() const
{
	if (isFront()) {
		return line->getBackNode();
	}
	else {
		return line->getFrontNode();
	}
}
int EditableLineNode::getID() const
{
	return id;
}
