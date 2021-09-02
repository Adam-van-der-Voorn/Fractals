#pragma once
#include "AbsLine.h"
#include <memory>
#include <vector>
#include "EditableLineNode.h"
#include "LFLine.h"

/**
Used to build a line fractal in the editing state.
Is made up of two selectable and transformable nodes- one for each endpoint on the line.
Only one node can be selected at a time.
**/
class EditableLine
{
public:
	EditableLine(int id, const AbsLine& line);

	/**
	 * Sets a node on this line to a position
	 * \param is_front true to set the front node's position, false to set the back node's position
	 * \param new_pos the new position of the specifed node
	*/
	void setNodePos(bool is_front, const Vec2& new_pos);

	/**
	 * Translates a node on this line
	 * \param is_front true to translate the front node, false to translate the back node
	 * \param translation the translation of the specfied node
	*/
	void translateNode(bool is_front, const Vec2& translation);

	/**
	\param is_front true to return the node at the front of this line
	false to return the node at the back
	\return a node on this line based on the given param
	**/
	const EditableLineNode* getNode(bool is_front) const;

	/**
	 * \return a vector of size 2 where the first element is the back node, and the last is the front
	*/
	const std::vector<const EditableLineNode*> getNodes() const;

	/**
	Gets the angle of this line, measured from the specified node
	\param from_front true to measure the angle from the front node,
	false to measure from the back
	\return the angle of this nodes line
	**/
	double getAngle(bool from_front) const;

	/**
	\return the length of this line
	**/
	double getLength() const;

	/**
	Sets the recusion status for this line.
	If true the line will recurse.
	**/
	void setRecursive(bool b);

	/**
	\return true if the line is recursive
	**/
	bool isRecursive() const;

	/**
	\param base_line the line that the LFLine will be based off
	\return a line fractal line relative to the given baseline
	**/
	LFLine toLFLine(AbsLine base_line) const;

	/**
	\return the absolute line
	**/
	AbsLine toAbsLine() const;

	/**
	\return this lines ID
	**/
	int getID() const;
	
private:
	// getNode but allows access
	EditableLineNode* node(bool is_front);
	int id;
	EditableLineNode back_node;
	EditableLineNode front_node;
	bool recursive = true;
};














