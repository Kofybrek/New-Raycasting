#include <array>
#include <chrono>
#include <cmath>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Animation.hpp"

//I explained how this work in this video: https://youtu.be/Df-U3BjlnDo
//Yeah, I'm promoting my channel here.

Animation::Animation(const std::string& i_sprite_name, SpriteManager& i_sprite_manager, const bool i_ping_pong, const float i_animation_speed, const float i_current_frame) :
	ping_pong(i_ping_pong),
	sprite_data(&i_sprite_manager.get_sprite_data(i_sprite_name)),
	sprite_manager(&i_sprite_manager)
{
	if (0 == ping_pong)
	{
		total_frames = sprite_data->total_frames;
	}
	else
	{
		total_frames = 2 * (sprite_data->total_frames - 1);
	}

	set_animation_speed(i_animation_speed);

	set_current_frame(i_current_frame);
}

bool Animation::update(const char i_animation_end)
{
	//We're gonna return whether or not the animation ended.
	//If the animation is going backwards and it reached the first frame, the left_end will be 1.
	bool left_end = 0;
	//If the animation is going forward and it reached the last frame, the right_end will be 1.
	bool right_end = 0;

	if (0 == ping_pong)
	{
		left_end = 0 > animation_speed + current_frame;
		right_end = total_frames <= animation_speed + current_frame;
	}
	else
	{
		//You have NO IDEA how long it took me to figure this out.
		left_end = 0 > animation_speed + current_frame || total_frames <= animation_speed + current_frame;
		right_end = current_frame < sprite_data->total_frames && sprite_data->total_frames <= animation_speed + current_frame;

		if (0 == right_end)
		{
			right_end = current_frame >= sprite_data->total_frames && sprite_data->total_frames > animation_speed + current_frame;
		}
	}

	current_frame = std::fmod(animation_speed + current_frame + total_frames, total_frames);

	switch (i_animation_end)
	{
		case -1:
		{
			return left_end;
		}
		case 0:
		{
			return left_end || right_end;
		}
		case 1:
		{
			return right_end;
		}
	}
  return false;
}

float Animation::get_animation_speed() const
{
	return animation_speed;
}

float Animation::get_current_frame() const
{
	return current_frame;
}

void Animation::draw(const sf::Vector2<short>& i_position, sf::RenderWindow& i_window, const bool i_mirror_horizontal, const bool i_mirror_vertical, const float i_scale_x, const float i_scale_y, const sf::Color& i_color, const sf::Rect<unsigned short>& i_texture_box) const
{
	unsigned short frame = floor(current_frame);

	//We're treating the ping pong animation as a regular one with twice as many frames to make things easier.
	if (1 == ping_pong && frame >= sprite_data->total_frames)
	{
		frame -= 2 * (1 + frame - sprite_data->total_frames);
	}

	sprite_manager->draw_sprite(frame, sprite_data->name, i_position, i_window, i_mirror_horizontal, i_mirror_vertical, i_scale_x, i_scale_y, i_color, i_texture_box);
}

void Animation::set_animation_speed(const float i_animation_speed)
{
	animation_speed = std::fmod(i_animation_speed, total_frames);
}

void Animation::set_current_frame(const float i_current_frame)
{
	current_frame = fmod(total_frames + fmod(i_current_frame, total_frames), total_frames);
}
