#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Headers/AngleFunctions.hpp"
#include "Headers/Global.hpp"
#include "Headers/MapCollision.hpp"
#include "Headers/Player.hpp"

Player::Player(const float i_x, const float i_y) :
	direction(0, 0),
	position(i_x, i_y)
{
	
}

void Player::set_position(const float i_x, const float i_y)
{
	position.x = i_x;
	position.y = i_y;
}

void Player::update(const sf::RenderWindow& i_window, const gbl::MAP::Map<>& i_map)
{
	if (1 == i_window.hasFocus())
	{
		float rotation_horizontal = 0;
		float rotation_vertical = 0;
		float step_x = 0;
		float step_y = 0;

		unsigned short window_center_x = static_cast<unsigned short>(round(0.5f * i_window.getSize().x));
		unsigned short window_center_y = static_cast<unsigned short>(round(0.5f * i_window.getSize().y));

		rotation_horizontal = gbl::RAYCASTING::FOV_HORIZONTAL * (window_center_x - sf::Mouse::getPosition(i_window).x) / i_window.getSize().x;
		rotation_vertical = gbl::RAYCASTING::FOV_VERTICAL * (window_center_y - sf::Mouse::getPosition(i_window).y) / i_window.getSize().y;

		direction.x = get_degrees(direction.x + rotation_horizontal);
		direction.y = std::clamp<float>(direction.y + rotation_vertical, -gbl::RAYCASTING::MAX_VERTICAL_DIRECTION, gbl::RAYCASTING::MAX_VERTICAL_DIRECTION);

		sf::Mouse::setPosition(sf::Vector2i(window_center_x, window_center_y), i_window);

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			step_x = gbl::PLAYER::MOVEMENT_SPEED * cos(deg_to_rad(90 + direction.x));
			step_y = -gbl::PLAYER::MOVEMENT_SPEED * sin(deg_to_rad(90 + direction.x));
		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			step_x = gbl::PLAYER::MOVEMENT_SPEED * cos(deg_to_rad(direction.x - 90));
			step_y = -gbl::PLAYER::MOVEMENT_SPEED * sin(deg_to_rad(direction.x - 90));
		}

		if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			step_x -= gbl::PLAYER::MOVEMENT_SPEED * cos(deg_to_rad(direction.x));
			step_y += gbl::PLAYER::MOVEMENT_SPEED * sin(deg_to_rad(direction.x));
		}
		else if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			step_x += gbl::PLAYER::MOVEMENT_SPEED * cos(deg_to_rad(direction.x));
			step_y -= gbl::PLAYER::MOVEMENT_SPEED * sin(deg_to_rad(direction.x));
		}

		if (0 == map_collision(step_x + position.x, step_y + position.y, i_map))
		{
			position.x += step_x;
			position.y += step_y;
		}
		else if (0 == map_collision(step_x + position.x, position.y, i_map))
		{
			position.x += step_x;
			position.y = round(position.y);
		}
		else if (0 == map_collision(position.x, step_y + position.y, i_map))
		{
			position.x = round(position.x);
			position.y += step_y;
		}
		else
		{
			position.x = round(position.x);
			position.y = round(position.y);
		}
	}
}

sf::Vector2f Player::get_direction() const
{
	return direction;
}

sf::Vector2f Player::get_position() const
{
	return position;
}