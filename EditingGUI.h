#pragma once
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Observer.h"

class Editing;
class EditableLineNode;

class EditingGUI : public sf::Drawable, Observer
{
public:
	EditingGUI(Editing* editing);
	void onNotify(int event_num);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	void updateNodes();
	void updateLines();
	Editing* editing;
	sf::VertexArray nodeLines = sf::VertexArray(sf::Lines);
	sf::VertexArray baseLine = sf::VertexArray(sf::Lines, 2);
	std::vector<sf::CircleShape> nodes;
	sf::VertexArray fractalLines = sf::VertexArray(sf::Lines);
	
};

