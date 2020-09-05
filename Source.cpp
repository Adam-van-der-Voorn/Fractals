#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "KochSnowflake.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

float scale_multi = 0.2;

int main()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(1300, 600), "asd", sf::Style::Default, settings);
	KochSnowflake snowflake(sf::Vector2f(-600, 0), sf::Vector2f(600, 0), &window);
	snowflake.setOrigin(sf::Vector2f(window.getSize().x / 2, window.getSize().y - 30));
	snowflake.generate(9);
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::MouseWheelScrolled) {
				float multi = 1 + (event.mouseWheelScroll.delta*scale_multi);
				sf::Vector2f mouse_pos = sf::Vector2f(event.mouseWheelScroll.x, event.mouseWheelScroll.y);
				snowflake.scaleMult(multi, mouse_pos);
				std::cout << "sf scale: " << snowflake.getScale() << std::endl;
			}
			else if (event.type == sf::Event::MouseButtonPressed){
				snowflake.setOrigin(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
			}
		}
		window.clear();
		const sf::VertexArray* line = snowflake.getLine();
		window.draw(*line);
		window.display();
	}
	return 0;
}