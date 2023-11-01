#include <array>
#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/AngleFunctions.hpp"
#include "Headers/Global.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Stripe.hpp"

Stripe::Stripe() :
	angle(0),
	distance(0),
	side_x(0),
	true_distance(0),
	side(0),
	x(0),
	sprite_name("WALL")
{
	
}

bool Stripe::operator>(const Stripe& i_stripe) const
{
	return distance > i_stripe.get_distance();
}

bool Stripe::operator<(const Stripe& i_stripe) const
{
	return distance < i_stripe.get_distance();
}

float Stripe::get_angle() const
{
	return angle;
}

float Stripe::get_distance() const
{
	return distance;
}

float Stripe::get_true_distance() const
{
	return true_distance;
}

short Stripe::get_y() const
{
	return round(0.5f * (gbl::SCREEN::HEIGHT - get_height()));
}

unsigned short Stripe::get_height() const
{
	return static_cast<unsigned short>(round(gbl::SCREEN::HEIGHT / (distance * tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_VERTICAL)))));
}

unsigned short Stripe::get_side_x() const
{
	return floor(side_x * sprite_manager->get_sprite_data(sprite_name).texture_box.width);
}

void Stripe::draw(const short i_pitch, sf::RenderWindow& i_window) const
{
	unsigned char shade = 255 * std::clamp<float>(1 - distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

	if (0 < shade)
	{
		sprite_manager->draw_sprite(0, sprite_name, sf::Vector2<short>(x, i_pitch + get_y()), i_window, 0, 0, 1, get_height() / static_cast<float>(sprite_manager->get_sprite_data(sprite_name).texture_box.height), sf::Color(shade, shade, shade), sf::Rect<unsigned short>(get_side_x(), 0, 1, USHRT_MAX));
	}
}

void Stripe::set_angle(const float i_angle)
{
	angle = i_angle;
}

void Stripe::set_distance(const float i_distance)
{
	distance = i_distance;
}

void Stripe::set_side(const unsigned char i_side)
{
	side = i_side;
}

void Stripe::set_side_x(const float i_side_x)
{
	side_x = i_side_x;
}

void Stripe::set_sprite_manager(SpriteManager& i_sprite_manager)
{
	sprite_manager = &i_sprite_manager;
}

void Stripe::set_sprite_name(const std::string& i_sprite_name)
{
	sprite_name = i_sprite_name;
}

void Stripe::set_true_distance(const float i_true_distance)
{
	true_distance = i_true_distance;
}

void Stripe::set_x(const unsigned short i_x)
{
	x = i_x;
}