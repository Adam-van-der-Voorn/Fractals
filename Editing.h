#pragma once
#include "Subject.h"
#include "Vec2.h"
#include "EventHandler.h"
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <unordered_set>
#include <memory>


class EditableLineNode;
class EditableLine;
class EditingGUI;
class LineFractal;
class StateMachine;
class EditingState;
struct Vec2;

class Editing : public Subject<Editing>, public EventHandler
{
public:
	Editing(EditingState* state, LineFractal* fractal);

	/**
	\return the moveable nodes of the foundational lines.
	**/
	const std::unordered_map<int, EditableLineNode*>& getNodes() const;

	/**
	\return the foundational lines of the fractal (ie the lines that are used in the recursion for the fractal)
	**/
	const std::unordered_map<int, std::shared_ptr<EditableLine>>& getLines() const;

	/**
	\return the base line for the fractal
	**/
	const std::shared_ptr<EditableLine> getBaseLine() const;

	/**
	\return th ids of all the nodes that are selected
	**/
	const std::unordered_set<int>& getSelectedNodes() const;

	/**
	\the fractal currently being edited
	**/
	const LineFractal* getFractal() const;

	/**
	\return the maximum amount of recursions this editor is allowing the fractal.
	a return value of anything under 0 means there is no limit set 
	**/
	int getNumRecursions() const;

	/**
	sets the maximum amount of recursions this editor will allow the fractal
	if set below 0 no limit will be set
	\param num the number of recursions
	**/
	void setNumRecursions(int num);

	/**
	\return the size of the editing frame
	**/
	Vec2 getEditingFrameSize() const;

	/**
	\return the most recent position of the mouse in the editing frame
	**/
	Vec2 getMousePosInFrame() const;

	/**
	\return a pointer to the clipboard
	**/
	double* getValClipboard();

	/** 
	recalculates the center of the editing frame.
	\param dimensions the dimensions of the window
	**/
	void recalcEditingFrameDimensions(Vec2 window_dimensions);

	/**
	updates the fractal
	**/
	void updateFractal();

	/**
	Moves a node by a given amount.
	node movement is restricted to ensure line length is a little less than the base line
	\param node_id the id of the node to move
	\param translation amount to move
	**/
	void moveNode(int node_id, Vec2 translation);

	/**
	Sets the hovered node to the node with the given id
	\param node_id the id of the node
	**/
	void setHoveredNode(int node_id);

	/**
	\return the id of the hovered node, or a val < 0 if no node is being hovered
	**/
	int getHoveredNode() const;

	/**
	sets the hovered node state to no node being hovered over
	**/
	void clearHoveredNode();

	/**
	removes all other nodes from selection apart from the hovered node
	**/
	void selectOnlyHoveredNode();

	/**
	\return true if a node is being hovered over
	**/
	bool nodeIsHovered() const;

	/**
	adds a line to the editing surface
	**/
	void addLine();

	/**
	removes all selected lines from the editing surface
	**/
	void removeSelectedLines();

	/**
	sets the given nodes position
	\param node_id the id of the node to change
	\param pos the new position of that node
	**/
	void setNodePosition(int node_id, Vec2 pos);

	/**
	sets the line angle of the given node
	\param node_id the id of the node to change
	\param pos the new line angle
	**/
	void setNodeAngle(int node_id, double angle);

	/**
	sets the line length of the given node
	\param node_id the id of the node to change
	\param pos the new line length
	**/
	void setNodeLength(int node_id, double length);

	/**
	sets the given lines recursiveness 
	\param line_id the id of the line to change
	\param b true to make the line recursiveness
	**/
	void setLineRecursiveness(int line_id, bool b);

	/**
	\return the global transform
	**/
	Vec2 getGlobalOffset() const;
	
	// inherited via EventHandler
	void handleEvent(sf::Event& event) override;

	/**
	Events to be sent to observers when something of note happens
	**/
	enum Event{
		LINES_CHANGED, // lines being moved, added or removed,
		SELECTION_CHANGED, // any change of selection (not including chnage in hovered node)
		FRACTAL_CHANGED, // if the fractal has changed in any way
		MOUSE_MOVED, 
		HOVERED_NODE_CHANGED 
	};

	// the width of the right panel
	const int right_panel_width = 200;

	// convinence constants for the gui
	const int general_padding = 6;
	const int general_element_width = right_panel_width - (general_padding * 2);
private:
	bool isWithinEditingFrame(Vec2 point) const;

	EditingState* state;

	// frame contents ///
	std::unordered_map<int, EditableLineNode*> nodes;
	std::unordered_set<int> selected_nodes;
	std::unordered_map<int, Vec2> dragging_nodes;
	std::unordered_map<int, std::shared_ptr<EditableLine>> lines;
	std::shared_ptr<EditableLine> base_line;
	LineFractal* fractal;
	int num_recursions = 7;
	// the id of the node that the user is hovering over in the gui
	int hovered_node;

	double value_clipboard = 0.0;

	// data for frame actions //
	bool mouse_moved_since_lpress = false;
	Vec2 left_press_location;

	// frame data //
	Vec2 editing_frame_size;
	Vec2 mouse_framepos;
	// global transform for the contents of the editing pane.
	Vec2 global_offset;
};

