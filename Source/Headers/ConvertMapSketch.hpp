#pragma once

#include "Decoration.hpp"
#include "Global.hpp"
#include "Player.hpp"
#include "SpriteManager.hpp"
#include "Steven.hpp"
#include <vector>

gbl::MAP::Map<> convert_map_sketch(std::vector<Decoration>& i_decorations, Player& i_player, StevenIzDaBest& i_steven, SpriteManager& i_sprite_manager);
