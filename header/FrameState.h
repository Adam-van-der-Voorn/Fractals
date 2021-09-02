#pragma once

#include <unordered_map>
#include "EditableLineNode.h"
#include "EditableLine.h"
#include "LineFractal.h"
#include "NodeID.h"
#include <unordered_set>
#include <memory>

class FrameState {
    private:
        // pair represents a selected node: <line_id, isFrontNode>
        std::unordered_set<NodeID> selected_nodes;
        std::unordered_map<int, EditableLine> lines;
        std::unordered_map<NodeID, const EditableLineNode*> nodes;
        EditableLine base_line;
        std::vector<AbsLine> fractal;

    public:
        FrameState(const AbsLine& base_line);

        /**
         * Add a line to the frame
         * \param line_dimensions the dimensions of the line to add
        */
        void addLine(const AbsLine& line);

        /**
         * Remove a line
         * \param id the id of the line to remove
        */
        void removeLine(int id);

        /**
        \param id the id of the node to reposition
        \param position the new position of the node
        */
        void setNodePos(NodeID node_id, Vec2 position);

        /**
         * \param id the id of the line to change
         * \param isRecursive true if the line should recurse
         */
        void setLineRecursiveness(int id, bool isRecursive);

        /**
         * \param fractal fractal to set
        */
        void setFractal(const std::vector<AbsLine>& fractal);

        /**
         * Clears all the selected nodes
        */
        void clearSelection();

        /**
         * Selects the node with the given id
         * \param id the id of the node to select
        */
        void selectNode(NodeID node_id);

        /**
         * \param node_id the id of the node
         * \return the node corresponding to the given id
        */
        const EditableLineNode* getNode(NodeID node_id) const;

        /**
         * \return a map of all the nodes on the frame, mapped to thier ids
        */
        const std::unordered_map<NodeID, const EditableLineNode*>& getNodes() const;

        /**
         * \param line_id the id of the line
         * \return the node corresponding to the given id
        */
        const EditableLine& getLine(int line_id) const;

        /**
        \return the foundational lines of the fractal (ie the lines that are used in the recursion for the fractal)
        **/
        const std::unordered_map<int, EditableLine>& getLines() const;

        /**
        \return the base line for the fractal
        **/
        const EditableLine& getBaseLine() const;

        /**
        \return th ids of all the nodes that are selected
        **/
        const std::unordered_set<NodeID>& getSelectedNodes() const;

        /**
        \return the fractal currently being displayed
        **/
	    const std::vector<AbsLine>& getFractal() const;

};
	