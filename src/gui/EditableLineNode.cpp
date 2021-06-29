#include "EditableLineNode.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "LFLine.h"
#include <cassert>

EditableLineNode::EditableLineNode(int id, const Vec2& pos, bool is_front, EditableLine* line) :
id(id), pos(pos), is_front(is_front), line(line)
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

Vec2 EditableLineNode::getPosition() const
{
	return pos;
}

double EditableLineNode::getLength() const
{
	return lineLength(getLine()->toAbsLine());
}

double EditableLineNode::getAngle() const
{
	const EditableLineNode* other = getOtherNode();
	return lineAngle({pos, other->getPosition()});
}

bool EditableLineNode::isFront() const
{
	return is_front;
}

const EditableLine* EditableLineNode::getLine() const
{
	return line;
}

const EditableLineNode* EditableLineNode::getOtherNode() const
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
