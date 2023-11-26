#pragma once

// I just realized that this is the first time you're seeing the SpriteManager
// code. Sooooo... what do you think?
#include "Global.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <climits>
#include <string>
#include <unordered_map>
#include <vector>


class SpriteManager {
  std::unordered_map<std::string, gbl::SpriteData> sprites_data;

  std::vector<sf::Sprite> sprites;

  std::vector<sf::Texture> textures;

public:
  SpriteManager();

  void draw_sprite(const unsigned short i_frame,
                   const std::string &i_sprite_name,
                   const sf::Vector2<short> &i_position,
                   sf::RenderWindow &i_window,
                   const bool i_mirror_horizontal = 0,
                   const bool i_mirror_vertical = 0, const float i_scale_x = 1,
                   const float i_scale_y = 1,
                   const sf::Color &i_color = sf::Color(255, 255, 255),
                   const sf::Rect<unsigned short> &i_texture_box =
                       sf::Rect<unsigned short>(0, 0, USHRT_MAX, USHRT_MAX));
  void load_texture(const unsigned short i_texture_id,
                    const std::string &i_file_name);

  gbl::SpriteData &get_sprite_data(const std::string &i_sprite_name);
};
