#pragma once

#include "Global.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
class Player
{
	sf::Vector2f direction;
	sf::Vector2f position;
public:
	Player(const float i_x = 0, const float i_y = 0);

	void set_position(const float i_x, const float i_y);
	void update(const sf::RenderWindow& i_window, const gbl::MAP::Map<>& i_map);

	sf::Vector2f get_direction() const;
	sf::Vector2f get_position() const;
};
