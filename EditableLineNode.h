#pragma once
#include <memory>
#include "AbsLine.h"
#include "Vec2.h"

struct LFLine;
class EditableLine;

class EditableLineNode
{
public:

	EditableLineNode(int id, Vec2 pos, bool is_front, EditableLine* line);

	/**
	\param point the point to check
	\return true if the distance between the given position and start node < node size
	**/
	bool pointIntersection(Vec2 point) const;

	/**
	Translates the selected node by the given amount
	\param translation the horizontal translation
	**/
	void translate(Vec2 translation);

	/**
	Sets the position of the selected node
	\param position the new position
	**/
	void setPosition(Vec2 position);

	/**
	Sets the length of this nodes line.
	The position of this nodes partner node is changed to achieve this.
	\param length the new length of the line
	**/
	void setLength(double length);

	/**
	Sets the angle of this nodes line.
	The position of this nodes partner node is changed to achieve this.
	\param angle the new angle of the line
	**/
	void setAngle(double angle);

	/**
	\return the position of this node
	**/
	Vec2 getPosition() const;

	/**
	\return the angle from this node to its partner
	**/
	double getLength() const;

	/**
	\return the angle from this node to its partner
	**/
	double getAngle() const;

	/**
	\return true if this node is at the front of it's line;
	**/
	bool isFront() const;

	/**
	\return the line this node is a part of
	**/
	EditableLine* getLine() const;

	/**
	\return the other node on this nodes line
	**/
	EditableLineNode* getOtherNode() const;

	/**
	\return the id of the node
	**/
	int getID() const;

	static constexpr double NODE_RADIUS = 10;

private:
	int id;
	Vec2 pos;
	EditableLine* line;
	bool is_front;
};

