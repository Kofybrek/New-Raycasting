#pragma once

class Game
{
	bool show_map;

	float hand_offset;

	std::array<Stripe, gbl::SCREEN::WIDTH> stripes;

	std::vector<Decoration> decorations;

	sf::Event event;

	sf::RenderWindow window;

	sf::VertexArray fov_visualization;

	SpriteManager sprite_manager;

	Animation hand_animation;

	Player player;

	StevenIzDaBest steven;

	gbl::MAP::Map<> map;
public:
	Game();

	bool is_open() const;

	void calculate_fov_visualization();
	void draw();
	void draw_map();
	void handle_events();
	void raycast();
	void set_title(const std::string& i_title);
	void update();
};