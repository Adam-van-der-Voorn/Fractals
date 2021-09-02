#include "EditableLineNode.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "LFLine.h"
#include <cassert>

EditableLineNode::EditableLineNode(const Vec2& pos, bool is_front) :
pos(pos), is_front(is_front)
{
}

bool EditableLineNode::pointIntersection(Vec2 point) const
{
	if (distanceBetweenAB(point.x, point.y, pos.x, pos.y) <= NODE_RADIUS) {
		return true;
	}
	return false;
}

void EditableLineNode::setPosition(Vec2 position)
{
	pos = position;
}

Vec2 EditableLineNode::getPosition() const
{
	return pos;
}

bool EditableLineNode::isFront() const
{
	return is_front;
}
