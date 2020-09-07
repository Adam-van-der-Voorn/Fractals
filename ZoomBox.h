#pragma once
#include <SFML\Graphics.hpp>
#ifdef _DEBUG
#  include <iostream>
#  define PRINT(x) std::cout << x << std::endl;
#else
#  define PRINT(x) while(false){}
#endif

//TODO
class ZoomBox
{
private:
	sf::Vector2f start_point;
	sf::Vector2f end_point;
	sf::RectangleShape rect;
	const sf::RenderWindow* window;
	bool active;

	sf::Vector2f getFinalBoxSize() const;
public:
	ZoomBox(const sf::RenderWindow* window);
	void setStartPoint(sf::Vector2f new_start_point);
	void setEndPoint(sf::Vector2f new_end_point);
	float getZoomMulti() const;
	sf::Vector2f getZoomPoint() const;
	bool isActive() const;
	void setUnactive();
	const sf::RectangleShape* getRect();
};

