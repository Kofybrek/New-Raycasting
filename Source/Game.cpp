#include <array>
#include <chrono>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/AngleFunctions.hpp"
#include "Headers/SpriteManager.hpp"
#include "Headers/Animation.hpp"
#include "Headers/Stripe.hpp"
#include "Headers/Decoration.hpp"
#include "Headers/Player.hpp"
#include "Headers/Steven.hpp"
#include "Headers/ConvertMapSketch.hpp"
#include "Headers/Game.hpp"

Game::Game() :
	show_map(1),
	hand_offset(-0.5f),
	window(sf::VideoMode(gbl::SCREEN::RESIZE * gbl::SCREEN::WIDTH, gbl::SCREEN::RESIZE * gbl::SCREEN::HEIGHT), "Raycasting", sf::Style::Close),
	fov_visualization(sf::TriangleFan, 1 + gbl::SCREEN::WIDTH),
	hand_animation("HAND", sprite_manager, 0, gbl::SPRITES::FIRE_ANIMATION_SPEED),
	steven(sprite_manager)
{
	window.setMouseCursorVisible(0);
	window.setView(sf::View(sf::FloatRect(0, 0, gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT)));

	map = convert_map_sketch(decorations, player, steven, sprite_manager);

	steven.fill_map(map);

	for (Stripe& stripe : stripes)
	{
		stripe.set_sprite_manager(sprite_manager);
	}
}

bool Game::is_open() const
{
	return window.isOpen();
}

void Game::calculate_fov_visualization()
{
	float start_x = 0.5f + player.get_position().x;
	float start_y = 0.5f + player.get_position().y;

	fov_visualization[0].position = sf::Vector2f(gbl::MAP::CELL_SIZE * start_x, gbl::MAP::CELL_SIZE * start_y);

	for (unsigned short a = 0; a < gbl::SCREEN::WIDTH; a++)
	{
		fov_visualization[1 + a].position = sf::Vector2f(gbl::MAP::CELL_SIZE * (start_x + stripes[a].get_true_distance() * cos(stripes[a].get_angle())), gbl::MAP::CELL_SIZE * (start_y - stripes[a].get_true_distance() * sin(stripes[a].get_angle())));
	}
}

void Game::draw()
{
	if (0 == steven.get_screamer())
	{
		bool steven_is_drawn = 0;

		//I believe that by changing this, you can change your height in the game.
		//You can use that if you wanna add jumping or crouching.
		float camera_z = 0.5f * gbl::SCREEN::HEIGHT;
		float end_stripe_x = tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL)) * (1 - 2.f / gbl::SCREEN::WIDTH);
		float hand_offset_x = gbl::SPRITES::HAND_OFFSET_MAX * cos(gbl::RAYCASTING::PI * hand_offset);
		float hand_offset_y = 0.5f * gbl::SPRITES::HAND_OFFSET_MAX * sin(gbl::RAYCASTING::PI * abs(hand_offset));
		float ray_direction_end_x;
		float ray_direction_end_y;
		float ray_direction_start_x;
		float ray_direction_start_y;
		float start_stripe_x = -tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL));
		float vertical_fov_ratio = tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_VERTICAL));

		short pitch = round(0.5f * gbl::SCREEN::HEIGHT * tan(deg_to_rad(player.get_direction().y)));

		unsigned short decoration_index = 0;
		unsigned short floor_start_y = std::clamp<float>(pitch + 0.5f * gbl::SCREEN::HEIGHT, 0, gbl::SCREEN::HEIGHT);
		unsigned short hand_texture_height = gbl::SPRITES::HAND_SCALE * sprite_manager.get_sprite_data("HAND").texture_box.height;
		unsigned short hand_texture_width = gbl::SPRITES::HAND_SCALE * sprite_manager.get_sprite_data("HAND").texture_box.width;

		gbl::SpriteData floor_sprite_data = sprite_manager.get_sprite_data("FLOOR");

		sf::Image floor_image;
		floor_image.loadFromFile(floor_sprite_data.image_location);

		//We're gonna draw the floor in this buffer image because the floor is drawn pixel by pixel.
		//I tried drawing directly on the screen. I stopped when my PC started cooking eggs.
		sf::Image floor_buffer_image;
		floor_buffer_image.create(gbl::SCREEN::WIDTH, gbl::SCREEN::HEIGHT - floor_start_y);

		sf::Sprite floor_sprite;
		floor_sprite.setPosition(0, floor_start_y);

		sf::Texture floor_texture;

		ray_direction_end_x = cos(deg_to_rad(player.get_direction().x)) + end_stripe_x * cos(deg_to_rad(player.get_direction().x - 90));
		ray_direction_end_y = -sin(deg_to_rad(player.get_direction().x)) - end_stripe_x * sin(deg_to_rad(player.get_direction().x - 90));
		ray_direction_start_x = cos(deg_to_rad(player.get_direction().x)) + start_stripe_x * cos(deg_to_rad(player.get_direction().x - 90));
		ray_direction_start_y = -sin(deg_to_rad(player.get_direction().x)) - start_stripe_x * sin(deg_to_rad(player.get_direction().x - 90));

		window.clear();

		//Drawing the floor.
		//For some reason, this part hits heavily on the performance.
		//I couldn't find a way to optimize it, though.
		for (unsigned short a = floor_start_y; a < gbl::SCREEN::HEIGHT; a++)
		{
			float floor_step_x;
			float floor_step_y;
			float floor_x;
			float floor_y;
			float row_distance;

			//We're drawing the floor row by row from top to bottom.
			short row_y = a - pitch - 0.5f * gbl::SCREEN::HEIGHT;

			unsigned char shade;

			//Distance from the player to the current row we're drawing.
			row_distance = (0 == row_y) ? FLT_MAX : camera_z / (row_y * vertical_fov_ratio);

			shade = 255 * std::clamp<float>(1 - row_distance / gbl::RAYCASTING::RENDER_DISTANCE, 0, 1);

			if (0 < shade)
			{
				floor_step_x = row_distance * (ray_direction_end_x - ray_direction_start_x) / gbl::SCREEN::WIDTH;
				floor_step_y = row_distance * (ray_direction_end_y - ray_direction_start_y) / gbl::SCREEN::WIDTH;
				//You can get the position of the current floor cell we're drawing using these variables (floor(floor_x) and floor(floor_y)).
				//Then you'll be able to draw different floor textures.
				floor_x = 0.5f + player.get_position().x + ray_direction_start_x * row_distance;
				floor_y = 0.5f + player.get_position().y + ray_direction_start_y * row_distance;

				for (unsigned short b = 0; b < gbl::SCREEN::WIDTH; b++)
				{
					sf::Color floor_image_pixel = floor_image.getPixel(floor(floor_sprite_data.texture_box.width * (5 + floor_x - floor(floor_x))), floor(floor_sprite_data.texture_box.height * (floor_y - floor(floor_y))));

					floor_x += floor_step_x;
					floor_y += floor_step_y;

					floor_image_pixel *= sf::Color(shade, shade, shade);

					floor_buffer_image.setPixel(b, a - floor_start_y, floor_image_pixel);
				}
			}
		}

		floor_texture.loadFromImage(floor_buffer_image);

		floor_sprite.setTexture(floor_texture);

		window.draw(floor_sprite);

		calculate_fov_visualization();

		std::sort(decorations.begin(), decorations.end(), std::greater());
		std::sort(stripes.begin(), stripes.end(), std::greater());

		//Drawing things layer by layer.
		for (Stripe& stripe : stripes)
		{
			while (decoration_index < decorations.size() && stripe.get_distance() < decorations[decoration_index].get_distance())
			{
				//I'm repeating the same code 3 times.
				//I had 2 options:
				//1) Spending hours trying to figure out the efficient way of doing this.
				//2) Writing the same code 3 times and tolerating people making fun of me.
				if (0 == steven_is_drawn)
				{
					if (steven.get_distance() > decorations[decoration_index].get_distance())
					{
						steven_is_drawn = 1;

						steven.draw(pitch, window);
					}
				}

				decorations[decoration_index].draw(pitch, window);

				decoration_index++;
			}

			if (0 == steven_is_drawn)
			{
				if (steven.get_distance() > stripe.get_distance())
				{
					steven_is_drawn = 1;

					steven.draw(pitch, window);
				}
			}

			stripe.draw(pitch, window);
		}

		for (unsigned short a = decoration_index; a < decorations.size(); a++)
		{
			if (0 == steven_is_drawn)
			{
				if (steven.get_distance() > decorations[a].get_distance())
				{
					steven_is_drawn = 1;

					steven.draw(pitch, window);
				}
			}

			decorations[a].draw(pitch, window);
		}

		if (0 == steven_is_drawn)
		{
			steven.draw(pitch, window);
		}

		if (1 == show_map)
		{
			draw_map();
		}

		hand_animation.draw(sf::Vector2<short>(gbl::SCREEN::WIDTH + hand_offset_x - hand_texture_width, gbl::SCREEN::HEIGHT + hand_offset_y - hand_texture_height), window, 0, 0, gbl::SPRITES::HAND_SCALE, gbl::SPRITES::HAND_SCALE);

		window.display();
	}
	else
	{
		short screamer_x = 0.5f * (gbl::SCREEN::WIDTH - gbl::STEVEN::SCREAMER_RESIZE * sprite_manager.get_sprite_data("STEVEN_SCREAMER").texture_box.width);
		short screamer_y = gbl::STEVEN::SCREAMER_Y;

		//By the end of the project I became so lazy I didn't wanna bother working with the random library so I used rand().
		screamer_x += rand() % (1 + 2 * gbl::STEVEN::SCREAMER_MAX_OFFSET) - gbl::STEVEN::SCREAMER_MAX_OFFSET;
		screamer_y += rand() % (1 + 2 * gbl::STEVEN::SCREAMER_MAX_OFFSET) - gbl::STEVEN::SCREAMER_MAX_OFFSET;

		window.clear();

		sprite_manager.draw_sprite(0, "STEVEN_SCREAMER", sf::Vector2<short>(screamer_x, screamer_y), window, 0, 0, gbl::STEVEN::SCREAMER_RESIZE, gbl::STEVEN::SCREAMER_RESIZE);

		window.display();
	}
}

void Game::draw_map()
{
	float frame_angle = 360.f / sprite_manager.get_sprite_data("MAP_PLAYER").total_frames;
	float shifted_direction = get_degrees(player.get_direction().x + 0.5f * frame_angle);
	float steven_shifted_direction = get_degrees(steven.get_direction() + 0.5f * frame_angle);

	for (unsigned short a = 0; a < gbl::MAP::COLUMNS; a++)
	{
		for (unsigned short b = 0; b < gbl::MAP::ROWS; b++)
		{
			switch (map[a][b])
			{
				case gbl::MAP::Cell::Empty:
				{
					sprite_manager.draw_sprite(0, "MAP_CELL", sf::Vector2<short>(gbl::MAP::CELL_SIZE * a, gbl::MAP::CELL_SIZE * b), window);

					break;
				}
				default:
				{
					sprite_manager.draw_sprite(0, "MAP_WALL", sf::Vector2<short>(gbl::MAP::CELL_SIZE * a, gbl::MAP::CELL_SIZE * b), window);
				}
			}
		}
	}

	window.draw(fov_visualization);

	sprite_manager.draw_sprite(floor(steven_shifted_direction / frame_angle), "MAP_STEVEN", sf::Vector2<short>(round(gbl::MAP::CELL_SIZE * steven.get_position().x), round(gbl::MAP::CELL_SIZE * steven.get_position().y)), window);
	sprite_manager.draw_sprite(floor(shifted_direction / frame_angle), "MAP_PLAYER", sf::Vector2<short>(round(gbl::MAP::CELL_SIZE * player.get_position().x), round(gbl::MAP::CELL_SIZE * player.get_position().y)), window);
}

void Game::handle_events()
{
	while (1 == window.pollEvent(event))
	{
		switch (event.type)
		{
			case sf::Event::Closed:
			{
				window.close();

				break;
			}
			case sf::Event::KeyPressed:
			{
				switch (event.key.code)
				{
					case sf::Keyboard::M:
					{
						//I hate using boolean operators but if you like them, uncomment the next line.
						//show_map = !show_map;
						show_map = 1 - show_map;
					}
				}
			}
		}
	}
}

void Game::raycast()
{
	float ray_start_x = 0.5f + player.get_position().x;
	float ray_start_y = 0.5f + player.get_position().y;

	//It's almost impossible to explain everything here without visuals.
	//But I'll try my best.
	for (unsigned short a = 0; a < gbl::SCREEN::WIDTH; a++)
	{
		char cell_step_x = 0;
		char cell_step_y = 0;

		//Current stripe's x-coordinate relative to the direction of the player.
		float current_stripe_x = tan(deg_to_rad(0.5f * gbl::RAYCASTING::FOV_HORIZONTAL)) * (2 * a / static_cast<float>(gbl::SCREEN::WIDTH) - 1);
		//The angle between the player's direction and the direction to the current stripe.
		float ray_angle;
		//Ray's direction represented in coordinates (or distances. I don't know how to say it correctly).
		//I wanna say "vector" but I know some people will say "ACKULUQIDARTUALLY!!!!! A vector has a DIRECTION and a MAGNITUDE!" so I won't.
		float ray_direction_x = cos(deg_to_rad(player.get_direction().x)) + current_stripe_x * cos(deg_to_rad(player.get_direction().x - 90));
		float ray_direction_y = -sin(deg_to_rad(player.get_direction().x)) - current_stripe_x * sin(deg_to_rad(player.get_direction().x - 90));
		float side_x;
		//This ray checks for horizontal collisions.
		float x_ray_length = 0;
		float x_ray_unit_length = (0 == ray_direction_x) ? FLT_MAX : std::abs(1 / ray_direction_x);
		//This ray checks for vertical collisions.
		float y_ray_length = 0;
		float y_ray_unit_length = (0 == ray_direction_y) ? FLT_MAX : std::abs(1 / ray_direction_y);

		unsigned char current_cell_x = floor(ray_start_x);
		unsigned char current_cell_y = floor(ray_start_y);
		//The side of the wall.
		//When you're looking at the map from above.
		//0 - Left side.
		//1 - Top side.
		//2 - Right side.
		//3 - Bottom side.
		unsigned char side = 0;

		gbl::MAP::Cell collision_cell;

		//In order for the algorithm to work, the ray must start at the cell borders.
		//So if the starting position of the ray is not a cell border (which is very likely), we'll stretch it to the closest one.
		if (0 > ray_direction_x)
		{
			cell_step_x = -1;

			x_ray_length = x_ray_unit_length * (ray_start_x - current_cell_x);
		}
		else if (0 < ray_direction_x)
		{
			cell_step_x = 1;

			x_ray_length = x_ray_unit_length * (1 + current_cell_x - ray_start_x);
		}

		if (0 > ray_direction_y)
		{
			cell_step_y = -1;

			y_ray_length = y_ray_unit_length * (ray_start_y - current_cell_y);
		}
		else if (0 < ray_direction_y)
		{
			cell_step_y = 1;

			y_ray_length = y_ray_unit_length * (1 + current_cell_y - ray_start_y);
		}

		while (1)
		{
			//In case the ray hits a corner.
			//I'm a perfectionist (i. e. stupid).
			bool corner_collision = 0;

			//We stretch the shortest ray.
			if (x_ray_length < y_ray_length)
			{
				stripes[a].set_distance(x_ray_length);

				x_ray_length += x_ray_unit_length;

				current_cell_x += cell_step_x;

				if (1 == cell_step_x)
				{
					side = 2;
				}
				else
				{
					side = 0;
				}
			}
			else if (x_ray_length > y_ray_length)
			{
				stripes[a].set_distance(y_ray_length);

				y_ray_length += y_ray_unit_length;

				current_cell_y += cell_step_y;

				if (-1 == cell_step_y)
				{
					side = 3;
				}
				else
				{
					side = 1;
				}
			}
			else //If the rays are equal, that means we hit the corner, so we stretch both rays.
			{
				corner_collision = 1;

				stripes[a].set_distance(x_ray_length);

				x_ray_length += x_ray_unit_length;
				y_ray_length += y_ray_unit_length;

				current_cell_x += cell_step_x;
				current_cell_y += cell_step_y;

				if (1 == cell_step_x)
				{
					side = 2;
				}
				else
				{
					side = 0;
				}
			}

			//Making sure the current cell we're checking is inside our map.
			if (0 <= current_cell_x && 0 <= current_cell_y && gbl::MAP::COLUMNS > current_cell_x && gbl::MAP::ROWS > current_cell_y)
			{
				if (gbl::MAP::Cell::Empty != map[current_cell_x][current_cell_y])
				{
					collision_cell = map[current_cell_x][current_cell_y];

					break;
				}
				else if (1 == corner_collision)
				{
					//The ray can't go through 2 walls standing diagonally.
					if (gbl::MAP::Cell::Empty != map[current_cell_x - cell_step_x][current_cell_y] && gbl::MAP::Cell::Empty != map[current_cell_x][current_cell_y - cell_step_y])
					{
						collision_cell = map[current_cell_x - cell_step_x][current_cell_y];

						break;
					}
				}
			}
			else
			{
				break;
			}
		}

		//Once we hit the wall, we need to find where exactly we hit it to draw the texture correctly.
		if (0 == side % 2)
		{
			side_x = ray_start_y + ray_direction_y * stripes[a].get_distance();
		}
		else
		{
			side_x = ray_start_x + ray_direction_x * stripes[a].get_distance();
		}

		if (2 > side)
		{
			side_x = ceil(side_x) - side_x;
		}
		else
		{
			side_x -= floor(side_x);
		}

		stripes[a].set_angle(get_radians(atan2(-ray_direction_y, ray_direction_x)));
		stripes[a].set_side(side);
		stripes[a].set_side_x(side_x);
		stripes[a].set_x(a);

		ray_angle = stripes[a].get_angle() - deg_to_rad(player.get_direction().x);

		//We're calculating the perpendicular distance when casting rays. But we also need the Eukacfiragridalidian distance to visualize the FOV on the map.
		stripes[a].set_true_distance(stripes[a].get_distance() / abs(cos(ray_angle)));

		switch (collision_cell)
		{
			case gbl::MAP::Cell::HelpWall:
			{
				stripes[a].set_sprite_name("HELP_WALL");

				break;
			}
			case gbl::MAP::Cell::MugWall:
			{
				stripes[a].set_sprite_name("MUG_WALL");

				break;
			}
			case gbl::MAP::Cell::SkeletonWall:
			{
				stripes[a].set_sprite_name("SKELETON_WALL");

				break;
			}
			case gbl::MAP::Cell::TextWall:
			{
				stripes[a].set_sprite_name("TEXT_WALL");

				break;
			}
			case gbl::MAP::Cell::Wall:
			{
				stripes[a].set_sprite_name("WALL");
			}
		}
	}
}

void Game::set_title(const std::string& i_title)
{
	window.setTitle(i_title);
}

void Game::update()
{
	if (0 == steven.get_screamer())
	{
		float player_movement_distance;

		sf::Vector2f player_position = player.get_position();

		player.update(window, map);

		steven.update(window, player.get_direction(), player.get_position(), map);

		player_movement_distance = sqrt(pow(player_position.x - player.get_position().x, 2) + pow(player_position.y - player.get_position().y, 2));

		raycast();

		for (Decoration& decoration : decorations)
		{
			decoration.update(player.get_direction(), player.get_position());
		}

		hand_animation.update();

		//So this is what they mean when they say "Drawing hands is hard".
		if (0 < player_movement_distance)
		{
			hand_offset += gbl::SPRITES::HAND_BOBBING_SPEED * player_movement_distance / gbl::PLAYER::MOVEMENT_SPEED;

			while (1 <= hand_offset)
			{
				hand_offset -= 2;
			}
		}
		else
		{
			if (-0.5f > hand_offset)
			{
				hand_offset += gbl::SPRITES::HAND_BOBBING_SPEED;

				if (-0.5f < hand_offset)
				{
					hand_offset = -0.5f;
				}
			}
			else if (-0.5f < hand_offset && 0.5f > hand_offset)
			{
				if (0 > hand_offset)
				{
					hand_offset -= gbl::SPRITES::HAND_BOBBING_SPEED;

					if (-0.5f > hand_offset)
					{
						hand_offset = -0.5f;
					}
				}
				else
				{
					hand_offset += gbl::SPRITES::HAND_BOBBING_SPEED;

					if (0.5f < hand_offset)
					{
						hand_offset = 0.5f;
					}
				}
			}
			else if (0.5f < hand_offset)
			{
				hand_offset -= gbl::SPRITES::HAND_BOBBING_SPEED;

				if (0.5f > hand_offset)
				{
					hand_offset = 0.5f;
				}
			}

			hand_offset = -abs(hand_offset);
		}
	}
}