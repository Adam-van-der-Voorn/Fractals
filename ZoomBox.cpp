#include "ZoomBox.h"
#include "signum.h"
#include "algorithm"

ZoomBox::ZoomBox(const sf::RenderWindow* window): window(window) {
	rect.setFillColor(sf::Color::Transparent);
	rect.setOutlineThickness(1.0f);
	rect.setOutlineColor(sf::Color::Red);
}

void ZoomBox::setStartPoint(sf::Vector2f new_start_point)
{
	active = true;
	start_point = new_start_point;
	end_point = start_point;
}

void ZoomBox::setEndPoint(sf::Vector2f new_end_point)
{
	end_point = new_end_point;
}

float ZoomBox::getZoomMulti() const
{
	return window->getSize().x / getFinalBoxSize().x;
}

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
	//PRINT("top:" << top_of_box << " lef:" << left_of_box << " rig:" << right_of_box << " tlm:" << tl_grad << " trm:" << tr_grad << " trc:" << tr_yinter)
	intercept.x = (-tr_yinter) / (tr_grad - tl_grad);
	intercept.y = (tl_grad*intercept.x);
	return intercept;
}

bool ZoomBox::isActive() const
{
	return active;
}

void ZoomBox::setUnactive()
{
	active = false;
}

const sf::RectangleShape* ZoomBox::getRect()
{
	rect.setPosition(start_point);
	rect.setSize(getFinalBoxSize());
	return &rect;
}

sf::Vector2f ZoomBox::getFinalBoxSize() const {
	sf::Vector2f box_size = (end_point - start_point); // could be negative
	sf::Vector2f window_size = sf::Vector2f((float)(window->getSize().x), (float)(window->getSize().y));
	float window_gradient = window_size.y / window_size.x;

	float box_gradient = abs(box_size.y / box_size.x);
	//PRINT("Box size: (" << box_size.x << ", " << box_size.y << ") Box, Window gradient : " << box_gradient << ", " << window_gradient)
	sf::Vector2f final_box_size;
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


