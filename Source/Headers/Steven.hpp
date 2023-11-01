#pragma once

class StevenIzDaBest
{
	bool in_the_view;
	bool screamer;

	float direction;
	float distance;

	int screen_x;

	unsigned char current_frame;

	sf::Vector2f position;

	SpriteManager* sprite_manager;

	//----------------< A star algorithm start >----------------
	unsigned short astar_path_length;

	std::map<gbl::Position<>, gbl::Position<>> astar_previous_cell;

	std::vector<sf::Vector2<unsigned short>> astar_path_vector;

	sf::Vector2<unsigned short> next_cell;

	gbl::MAP::Map<gbl::MAP::PathCell> astar_map;

	gbl::MAP::Map<float> astar_f_scores;
	gbl::MAP::Map<float> astar_g_scores;
	gbl::MAP::Map<float> astar_h_scores;
	//----------------< A star algorithm end >----------------
public:
	StevenIzDaBest(SpriteManager& i_sprite_manager, const float i_x = 0, const float i_y = 0);

	bool get_screamer() const;

	float get_direction() const;
	float get_distance() const;

	int get_height() const;
	int get_width() const;
	int get_x() const;
	int get_y() const;

	void draw(const short i_pitch, sf::RenderWindow& i_window);
	void fill_map(const gbl::MAP::Map<>& i_map);
	void set_position(const float i_x, const float i_y);
	void update(const sf::RenderWindow& i_window, const sf::Vector2f& i_player_direction, const sf::Vector2f& i_player_position, const gbl::MAP::Map<>& i_map);

	sf::Vector2f get_position() const;
};