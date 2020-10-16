#pragma once
#include <SFML/Graphics/VertexArray.hpp>

inline void fillLineColour(sf::VertexArray& vert_arr, sf::Color color) {
	for (size_t i = 0; i < vert_arr.getVertexCount(); i++) {
		vert_arr[i].color = color;
	}
}