#pragma once
#include "SelLineWidget.h"
#include "Editing.h"
#include "Observer.h"
#include "NumFieldExt.h"
#include <TGUI/Widgets/Panel.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/Canvas.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <unordered_map>

class EditableLineNode;
class FrameState;

class SelLineWidget : public Observer<Editing>, public Observer<NumFieldExt> , public tgui::Panel
{
public:
	typedef std::shared_ptr<SelLineWidget> Ptr; //!< Shared widget pointer
	typedef std::shared_ptr<const SelLineWidget> ConstPtr; //!< Shared constant widget pointer

	SelLineWidget(Editing* editing, int node_id, float width);
	~SelLineWidget();
	SelLineWidget(const SelLineWidget&); 

	// Inherited via observer
	void onNotify(Editing* editing, int event_num) override;
	void onNotify(NumFieldExt* editing, int event_num) override;

	/**
	Redraws the icon to match the node paired with this widget
	**/
	void redrawIcon();

	// Inherited via tgui::Widget
	/**
	creates a copy of this Panel, even if its static type is widget.
	if you know the type is panel, you can use the copy constructor.
	**/
	tgui::Widget::Ptr clone() const override;

private:
	const float padding = 3;
	void init();
	int node_id;
	Editing* editing;
	FrameState* frame;
	tgui::Canvas::Ptr temp_background = tgui::Canvas::create();
	tgui::Button::Ptr select_button;
	tgui::Canvas::Ptr icon; 
	NumFieldExt::Ptr xpos_input;
	NumFieldExt::Ptr ypos_input;
	NumFieldExt::Ptr dir_input;
	NumFieldExt::Ptr len_input;
	tgui::CheckBox::Ptr rec_checkbox;
};

