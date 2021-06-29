#pragma once

#include <unordered_map>
#include "EditableLineNode.h"
#include "EditableLine.h"
#include "LineFractal.h"
#include <unordered_set>
#include <memory>

class FrameState {
    private:
        // line map stores actual lines, nodes map stores ptrs to the nodes in the lines
        std::unordered_map<int, EditableLineNode*> nodes;
        std::unordered_set<int> selected_nodes;
        std::unordered_map<int, EditableLine> lines;
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
        void setNodePos(int id, Vec2 position);

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
        void selectNode(int id);

        /**
        \return the moveable nodes of the foundational lines.
        **/
        const std::unordered_map<int, EditableLineNode*>& getNodes() const;

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
        const std::unordered_set<int>& getSelectedNodes() const;

        /**
        \return the fractal currently being displayed
        **/
	    const std::vector<AbsLine>& getFractal() const;

};
	