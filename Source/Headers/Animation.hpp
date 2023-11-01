#pragma once

class Animation
{
	//Does the animation go back and forth?
	bool ping_pong;

	float animation_speed;
	float current_frame;

	unsigned short total_frames;

	gbl::SpriteData* sprite_data;

	SpriteManager* sprite_manager;
public:
	Animation(const std::string& i_sprite_name, SpriteManager& i_sprite_manager, const bool i_ping_pong = 0, const float i_animation_speed = 1, const float i_current_frame = 0);
	
	bool update(const char i_animation_end = 0);

	float get_animation_speed() const;
	float get_current_frame() const;

	void draw(const sf::Vector2<short>& i_position, sf::RenderWindow& i_window, const bool i_mirror_horizontal = 0, const bool i_mirror_vertical = 0, const float i_scale_x = 1, const float i_scale_y = 1, const sf::Color& i_color = sf::Color(255, 255, 255), const sf::Rect<unsigned short>& i_texture_box = sf::Rect<unsigned short>(0, 0, USHRT_MAX, USHRT_MAX)) const;
	void set_animation_speed(const float i_animation_speed);
	void set_current_frame(const float i_current_frame);
};