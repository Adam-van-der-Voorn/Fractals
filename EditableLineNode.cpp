#include "EditableLineNode.h"
#include "EditableLine.h"
#include "vecutil.h"
#include "LFLine.h"

EditableLineNode::EditableLineNode(int id, double x, double y, bool is_front, EditableLine* line) : id(id), xpos(x), ypos(y), is_front(is_front), line(line)
{
}

bool EditableLineNode::pointIntersection(int x, int y) const
{
	if (distanceBetweenAB(x, y, xpos, ypos) <= NODE_RADIUS) {
		return true;
	}
	return false;
}

void EditableLineNode::translate(double dx, double dy)
{
	xpos += dx;
	ypos += dy;
}

void EditableLineNode::setPosition(double x, double y)
{
	xpos = x;
	ypos = y;
}

void EditableLineNode::setLength(double length)
{

}

void EditableLineNode::setAngle(double angle)
{
}

void EditableLineNode::setRecursive(bool front_node)
{

}

double EditableLineNode::getX() const
{
	return xpos;
}
double EditableLineNode::getY() const
{
	return ypos;
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
