#pragma once
#include <vector>
#include <memory>
#include <TGUI/TGUI.hpp>
#include "State.h"
#include "Subject.h"
#include "Vec2.h"
#include "RightAngleRect.h"
#include "LineFractal.h"

class ZoomBox;
class StateMachine;
class ViewingState;

class Viewing : public EventHandler, public Subject<Viewing>
{
public:
	Viewing(ViewingState* state);
	~Viewing();

	//inherited via EventHandler
	void handleEvent(sf::Event& event);

	//getters & setters
	ZoomBox* getZoomBox() const;

	/**
	\return the fractal being viewed
	**/
	const LineFractal& getFractal() const;

	/**
	Updates the bounding box of each fractal
	**/
	void updateFractalBounds();

	/**
	Increases or decreases the view window based on the relative size of the given rectangle
	in this case, the view window is the view window stored in the fractal in this class
	\param view_multi the dimesions of the new view relative to the current
	The w:h ratio of view_multi must be equal to the ratio of the view window.
	If the width of view_multi is smaller than the view window, this results in a more "zoomed in" view, and vice versa.
	**/
	void changeViewWindow(const RightAngleRect& view_multi);

	/**
	\return the fractal zoom
	**/
	double getFractalZoom() const;

	/**
	\return the fractal offset
	**/
	Vec2 getFractalOffset() const;

	/**
	resets the transform of the fractal back to its default values
	**/
	void resetFractalTransform();

	/**
	positions the fractal in the center of the screen, based off it's base lines bounding box
	does not work :(
	**/
	void centerFractal();

	/**
	Sets the viewed fractal to the given fractal
	**/
	void setFractal(const LineFractal& new_fractal);

	enum Event {
		ZOOM_BOX_CHANGE, VIEW_CHANGE
	};

private:
	ViewingState* state;
	LineFractal fractal = LineFractal({ {0,0}, {0,0} });
	ZoomBox* zoom_box;

	RightAngleRect current_view;

	// local transform for the fractal only
	double fractal_zoom = 1;
	Vec2 fractal_offset = {0, 0};
};

