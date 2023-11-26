#pragma once

#include "SpriteManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
class Stripe
{
	//Angle relative to the player's current direction.
	float angle;
	//Perpendicular distance.
	float distance;
	//Position in the wall texture.
	float side_x;
	//Euquackquacklidian distance.
	float true_distance;

	unsigned char side;

	//This should be screen_x. But it's not. For some reason.
	unsigned short x;

	std::string sprite_name;

	SpriteManager* sprite_manager;
public:
	Stripe();

	bool operator>(const Stripe& i_stripe) const;
	bool operator<(const Stripe& i_stripe) const;

	float get_angle() const;
	float get_distance() const;
	float get_true_distance() const;

	short get_y() const;

	unsigned short get_height() const;
	unsigned short get_side_x() const;

	void draw(const short i_pitch, sf::RenderWindow& i_window) const;
	void set_angle(const float i_angle);
	void set_distance(const float i_distance);
	void set_side(const unsigned char i_side);
	void set_side_x(const float i_side_x);
	void set_sprite_manager(SpriteManager& i_sprite_manager);
	void set_sprite_name(const std::string& i_sprite_name);
	void set_true_distance(const float i_true_distance);
	void set_x(const unsigned short i_x);
};
