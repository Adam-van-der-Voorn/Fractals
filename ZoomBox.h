#pragma once
#include <SFML\Graphics.hpp>
#include "Vec2.h"

class RightAngleRect;

class ZoomBox
{
private:
	Vec2 start_point = {0, 0};
	Vec2 end_point = {0, 0};
	bool active = false;
	const sf::Window* window;

	Vec2 getFinalBoxSize() const;
public:
	ZoomBox(const sf::Window* window);
	void setStartPoint(const Vec2& new_start_point);
	void setEndPoint(const Vec2& new_end_point);
	RightAngleRect getBox() const;
	/**
	\return true if the zoom box is NOT a line or point, but a proper rectangle
	**/
	bool isRect() const;
	bool isActive() const;
	void setUnactive();
};

