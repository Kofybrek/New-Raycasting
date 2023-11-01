#include <array>
#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/AngleFunctions.hpp"
#include "Headers/Global.hpp"
#include "Headers/Astar.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/MapCollision.hpp"
#include "Headers/Steven.hpp"

StevenIzDaBest::StevenIzDaBest(SpriteManager& i_sprite_manager, float i_x, float i_y) :
	in_the_view(0),
	screamer(0),
	direction(0),
	distance(0),
	screen_x(0),
	current_frame(0),
	position(i_x, i_y),
	sprite_manager(&i_sprite_manager),
	astar_path_length(0),
	next_cell(position),
	astar_map({}),
	astar_f_scores({}),
	astar_g_scores({}),
	astar_h_scores({})
{
	
}

bool StevenIzDaBest::get_screamer() const
{
	return screamer;
}

float StevenIzDaBest::get_direction() const
{
	return direction;
}

float StevenIzDaBest::get_distance() const
{
	return distance;
}

int StevenIzDaBest::get_height() const
{
	return round(gbl::SCREEN::HEIGHT / (distance * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_VERTICAL))));
}

int StevenIzDaBest::get_width() const
{
	float sprite_height = sprite_manager->get_sprite_data("STEVEN").texture_box.height;
	float sprite_width = sprite_manager->get_sprite_data("STEVEN").texture_box.width;

	return round(gbl::SCREEN::HEIGHT * sprite_width / (distance * sprite_height * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));
}

int StevenIzDaBest::get_x() const
{
	return screen_x - round(0.5f * get_width());
}

int StevenIzDaBest::get_y() const
{
	return round(0.5f * (gbl::SCREEN::HEIGHT - get_height()));
}

void StevenIzDaBest::draw(const short i_pitch, sf::RenderWindow& i_window)
{
	float sprite_height = sprite_manager->get_sprite_data("STEVEN").texture_box.height;
	float sprite_width = sprite_manager->get_sprite_data("STEVEN").texture_box.width;

	unsigned char shade = 255 * std::clamp<float>(1 - distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

	in_the_view &= gbl::SCREEN::HEIGHT > i_pitch + get_y() && gbl::SCREEN::WIDTH > get_x() && get_x() > -1 * get_width() && i_pitch + get_y() > -1 * get_height();

	if (0 < shade && 1 == in_the_view)
	{
		sprite_manager->draw_sprite(current_frame, "STEVEN", sf::Vector2<short>(get_x(), i_pitch + get_y()), i_window, 0, 0, get_width() / sprite_width, get_height() / sprite_height, sf::Color(shade, shade, shade));
	}
}

void StevenIzDaBest::fill_map(const gbl::MAP::Map<>& i_map)
{
	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			if (gbl::MAP::Cell::Empty != i_map[a][b])
			{
				astar_map[a][b] = gbl::MAP::PathCell::Wall;
			}
			else
			{
				astar_map[a][b] = gbl::MAP::PathCell::Empty;
			}
		}
	}
}

void StevenIzDaBest::set_position(const float i_x, const float i_y)
{
	position.x = i_x;
	position.y = i_y;

	next_cell.x = position.x;
	next_cell.y = position.y;
}

void StevenIzDaBest::update(const sf::RenderWindow& i_window, const sf::Vector2f& i_player_direction, const sf::Vector2f& i_player_position, const gbl::MAP::Map<>& i_map)
{
	if (1 == i_window.hasFocus())
	{
		float angle = get_radians(atan2(i_player_position.y - position.y, position.x - i_player_position.x));
		float difference = deg_difference(i_player_direction.x, rad_to_deg(angle));
		float frame_angle = 360.f / sprite_manager->get_sprite_data("STEVEN").total_frames;
		float shifted_direction;
		float speed = 0;
		float step_x = 0;
		float step_y = 0;

		//We're taking the cells that are closest to Steven and the player as starting and finishing cells.
		sf::Vector2<unsigned short> finish_position(round(i_player_position.x), round(i_player_position.y));
		sf::Vector2<unsigned short> start_position(round(position.x), round(position.y));

		if (finish_position != start_position)
		{
			astar_reset(astar_path_length, astar_previous_cell, astar_path_vector, astar_f_scores, astar_g_scores, astar_h_scores, finish_position, start_position, astar_map);
			astar_search(astar_path_length, astar_previous_cell, astar_path_vector, astar_f_scores, astar_g_scores, astar_h_scores, next_cell, finish_position, start_position, astar_map);

			direction = rad_to_deg(atan2(position.y - next_cell.y, next_cell.x - position.x));

			//The farther Steven is from the player, the faster he moves, and vice versa.
			if (gbl::STEVEN::MAX_MOVEMENT_DISTANCE < astar_path_length)
			{
				speed = gbl::STEVEN::MAX_MOVEMENT_SPEED;
			}
			else if (gbl::STEVEN::MIN_MOVEMENT_DISTANCE > astar_path_length)
			{
				speed = gbl::STEVEN::MIN_MOVEMENT_SPEED;
			}
			else
			{
				speed = gbl::STEVEN::MIN_MOVEMENT_SPEED + (gbl::STEVEN::MAX_MOVEMENT_SPEED - gbl::STEVEN::MIN_MOVEMENT_SPEED) * (astar_path_length - gbl::STEVEN::MIN_MOVEMENT_DISTANCE) / (gbl::STEVEN::MAX_MOVEMENT_DISTANCE - gbl::STEVEN::MIN_MOVEMENT_DISTANCE);
			}
		}
		
		if (1 > sqrt(pow(position.x - i_player_position.x, 2) + pow(position.y - i_player_position.y, 2)))
		{
			screamer = 1;
		}

		//Steven moves to the next cell in the A star path.
		if (next_cell.x < position.x)
		{
			step_x = std::max(-speed, next_cell.x - position.x);
		}
		else if (next_cell.x > position.x)
		{
			step_x = std::min(speed, next_cell.x - position.x);
		}

		if (next_cell.y < position.y)
		{
			step_y = std::max(-speed, next_cell.y - position.y);
		}
		else if (next_cell.y > position.y)
		{
			step_y = std::min(speed, next_cell.y - position.y);
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

		//This makes it so that the difference is between -180 to 180.
		if (deg_difference(i_player_direction.x, difference + rad_to_deg(angle)) < deg_difference(i_player_direction.x, rad_to_deg(angle) - difference))
		{
			difference *= -1;
		}

		shifted_direction = get_degrees(90 + get_degrees(direction + 0.5f * frame_angle) - difference - i_player_direction.x);

		//Calculating the perpendicular distance from Steven to the player.
		distance = abs(i_player_position.y - position.y - tan(deg_to_rad(i_player_direction.x - 90)) * (position.x - i_player_position.x)) / sqrt(1 + pow(tan(deg_to_rad(i_player_direction.x - 90)), 2));

		screen_x = round(0.5f * gbl::SCREEN::WIDTH * (1 - tan(deg_to_rad(difference)) / tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL))));

		in_the_view = 90 > abs(difference);

		current_frame = floor(shifted_direction / frame_angle);
	}
}

sf::Vector2f StevenIzDaBest::get_position() const
{
	return position;
}