#include "ZoomBox.h"
#include "signum.h"
#include <algorithm>
#include <cassert>

#include "RightAngleRect.h"

ZoomBox::ZoomBox(const sf::Window* window) : window(window)
{
}

void ZoomBox::setStartPoint(const Vec2& new_start_point)
{
	active = true;
	start_point = new_start_point;
	end_point = new_start_point;
}

void ZoomBox::setEndPoint(const Vec2& new_end_point)
{
	end_point = new_end_point;
}

RightAngleRect ZoomBox::getBox() const
{
	return RightAngleRect::fromSize(start_point, getFinalBoxSize());
}

/* might need coede later
sf::Vector2f ZoomBox::getZoomPoint() const
{
	// zoom point is the intersection of two lines.
	// one line goes from the tl corner of the window to the tl corner of the zoom box
	// the other goes from the tr corner of the window to the tr corner of the zoom box
	// get the gradient and y intercept of each line
	sf::Vector2f intercept;
	const sf::Vector2f box_size = getFinalBoxSize();
	float top_of_box = std::min(start_point.y, start_point.y + box_size.y);
	float left_of_box = std::min(start_point.x, start_point.x + box_size.x);
	float right_of_box = std::max(start_point.x, start_point.x + box_size.x);
	float tl_grad = top_of_box / left_of_box;
	float tr_grad = (-top_of_box) / (window->getSize().x - right_of_box);
	float tr_yinter = top_of_box - (tr_grad * right_of_box); // top left y intercept == 0 as it always passes though (0,0) 
	intercept.x = (-tr_yinter) / (tr_grad - tl_grad);
	intercept.y = (tl_grad*intercept.x);
	return intercept;
}
*/

bool ZoomBox::isRect() const
{
	//assert(active && "function only useful when box is active");
	if (start_point.x == end_point.x || start_point.y == end_point.y) {
		return false;
	}
	return true;
}

bool ZoomBox::isActive() const
{
	return active;
}

void ZoomBox::setUnactive()
{
	active = false;
}

Vec2 ZoomBox::getFinalBoxSize() const {
	Vec2 box_size = end_point - start_point; // could be negative
	Vec2 window_size = { window->getSize().x, window->getSize().y };
	float window_gradient = window_size.y / window_size.x;

	float box_gradient = abs(box_size.y / box_size.x);
	Vec2 final_box_size;
	if (window_gradient < box_gradient) { // window is wider than the box
		final_box_size.y = box_size.y;
		float y_ratio = window_size.x / window_size.y; // ratio of y:x where y = 1
		final_box_size.x = abs(final_box_size.y) * y_ratio * sign(box_size.x);
	}
	else if (window_gradient > box_gradient) { // window is taller than the box
		final_box_size.x = box_size.x;
		float x_ratio = window_size.y / window_size.x; // ratio of x:y where x = 1
		final_box_size.y = abs(final_box_size.x) * x_ratio * sign(box_size.y);
	}
	else {
		final_box_size = box_size;
	}
	return final_box_size;
}




