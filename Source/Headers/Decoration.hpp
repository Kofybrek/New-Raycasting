#pragma once

#include "Animation.hpp"
#include "SpriteManager.hpp"
#include <string>

class Decoration
{
	bool in_the_view;
	//I didn't wanna make another class for animated decorations.
	bool is_animation;

	float distance;

	int screen_x;

	std::string sprite_name;

	sf::Vector2f position;

	SpriteManager* sprite_manager;

	Animation animation;
public:
	Decoration(const bool i_is_animation, const std::string& i_sprite_name, SpriteManager& i_sprite_manager, const float i_x = 0, const float i_y = 0);

	bool operator>(const Decoration& i_decoration) const;
	bool operator<(const Decoration& i_decoration) const;

	float get_distance() const;

	int get_height() const;
	int get_width() const;
	int get_x() const;
	int get_y() const;

	void draw(const short i_pitch, sf::RenderWindow& i_window);
	void update(const sf::Vector2f& i_player_direction, const sf::Vector2f& i_player_position);
};
