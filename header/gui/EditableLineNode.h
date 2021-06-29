#pragma once
#include <memory>
#include "AbsLine.h"
#include "Vec2.h"

struct LFLine;
class EditableLine;

class EditableLineNode
{
public:

	EditableLineNode(int id, const Vec2& pos, bool is_front, EditableLine* line);

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
	Gets the length of this nodes line.
	\return the length of this nodes line
	**/
	double getLength() const;

	/**
	Gets the angle of this nodes line, measured from this node
	\return the angle of this nodes line
	**/
	double getAngle() const;

	/**
	\return the position of this node
	**/
	Vec2 getPosition() const;

	/**
	\return true if this node is at the front of it's line;
	**/
	bool isFront() const;

	/**
	\return the line this node is a part of
	**/
	const EditableLine* getLine() const;

	/**
	\return the other node on this nodes line
	**/
	const EditableLineNode* getOtherNode() const;

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

