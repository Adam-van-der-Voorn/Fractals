#include "EditableLine.h"
#include "SFML/Graphics/VertexArray.hpp"
#include "vecutil.h"

void EditableLine::drawTo(sf::RenderTarget& surface) const
{
	sf::VertexArray arr(sf::LinesStrip);
	arr.append(sf::Vertex(sf::Vector2f(x1, y1)));
	arr.append(sf::Vertex(sf::Vector2f(x2, y2)));
	surface.draw(arr);
}

EditableLine::EditableLine(int id, double x1, double y1, double x2, double y2) : 
	id(id), x1(x1), y1(y1), x2(x2), y2(y2) {}

bool EditableLine::attemptSelection(int x, int y)
{
	
}

void EditableLine::translate(double dx, double dy)
{
}

void EditableLine::setPosition(double x, double y)
{
}

void EditableLine::setLength(double length)
{
}

void EditableLine::setAngle(double angle)
{
}

void EditableLine::switchSelection()
{
}
