#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <SFML/Graphics.hpp>

#include "Headers/Global.hpp"
#include "Headers/SpriteManager.hpp"

SpriteManager::SpriteManager()
{
	unsigned short current_texture = 0;
	unsigned short total_textures = 0;

	for (const std::filesystem::directory_entry file : std::filesystem::directory_iterator("Resources/Images/"))
	{
		if (".sprites" == file.path().extension())
		{
			total_textures++;
		}
	}

	sprites.resize(total_textures);

	textures.resize(total_textures);

	//Adding new elements to the vector changes its memory address so we're resizing it then changing the existing ones.
	for (const std::filesystem::directory_entry file : std::filesystem::directory_iterator("Resources/Images/"))
	{
		if (".sprites" == file.path().extension())
		{
			load_texture(current_texture, file.path().stem().string());

			current_texture++;
		}
	}
}

void SpriteManager::draw_sprite(const unsigned short i_frame, const std::string& i_sprite_name, const sf::Vector2<short>& i_position, sf::RenderWindow& i_window, const bool i_mirror_horizontal, const bool i_mirror_vertical, const float i_scale_x, const float i_scale_y, const sf::Color& i_color, const sf::Rect<unsigned short>& i_texture_box)
{
	sf::Rect<short> texture_box(sprites_data.at(i_sprite_name).texture_box);
	texture_box.left += texture_box.width * (i_mirror_horizontal + i_frame);
	texture_box.top += i_mirror_vertical * texture_box.height;
	texture_box.height = std::min<unsigned short>(i_texture_box.height, texture_box.height);
	texture_box.width = std::min<unsigned short>(i_texture_box.width, texture_box.width);

	if (1 == i_mirror_horizontal)
	{
		texture_box.left -= i_texture_box.left;
		texture_box.width *= -1;
	}
	else
	{
		texture_box.left += i_texture_box.left;
	}

	if (1 == i_mirror_vertical)
	{
		texture_box.height *= -1;
		texture_box.top -= i_texture_box.top;
	}
	else
	{
		texture_box.top += i_texture_box.top;
	}

	//After some debugging I found out that this is SUPER inefficient. I'm planning to completely rewrite the SpriteManager because of it.
	sprites_data.at(i_sprite_name).sprite->setColor(i_color);
	sprites_data.at(i_sprite_name).sprite->setOrigin(sprites_data.at(i_sprite_name).origin.x, sprites_data.at(i_sprite_name).origin.y);
	sprites_data.at(i_sprite_name).sprite->setPosition(sf::Vector2f(i_position));
	sprites_data.at(i_sprite_name).sprite->setScale(i_scale_x, i_scale_y);
	sprites_data.at(i_sprite_name).sprite->setTextureRect(sf::IntRect(texture_box));

	i_window.draw(*sprites_data.at(i_sprite_name).sprite);
}

void SpriteManager::load_texture(const unsigned short i_texture_id, const std::string& i_file_name)
{
	unsigned char line_counter = 0;

	std::ifstream data_file("Resources/Images/" + i_file_name + ".sprites");

	std::string line;
	std::string name;

	textures[i_texture_id].loadFromFile("Resources/Images/" + i_file_name + ".png");

	sprites[i_texture_id].setTexture(textures[i_texture_id]);

	while (std::getline(data_file, line))
	{
		if (0 == line.size())
		{
			line_counter = 0;
		}
		else
		{
			switch (line_counter)
			{
				case 0:
				{
					name = line;

					sprites_data[name].name = name;
					sprites_data.at(name).image_location = "Resources/Images/" + i_file_name + ".png";
					sprites_data.at(name).sprite = &sprites[i_texture_id];

					break;
				}
				case 1:
				{
					sprites_data.at(name).origin.x = std::stoi(line);

					break;
				}
				case 2:
				{
					sprites_data.at(name).origin.y = std::stoi(line);

					break;
				}
				case 3:
				{
					sprites_data.at(name).texture_box.height = std::stoi(line);

					break;
				}
				case 4:
				{
					sprites_data.at(name).texture_box.width = std::stoi(line);

					break;
				}
				case 5:
				{
					sprites_data.at(name).texture_box.left = std::stoi(line);

					break;
				}
				case 6:
				{
					sprites_data.at(name).texture_box.top = std::stoi(line);

					break;
				}
				case 7:
				{
					sprites_data.at(name).total_frames = std::stoi(line);
				}
			}

			line_counter++;
		}
	}

	data_file.close();
}

gbl::SpriteData& SpriteManager::get_sprite_data(const std::string& i_sprite_name)
{
	return sprites_data.at(i_sprite_name);
}