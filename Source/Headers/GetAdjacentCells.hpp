#pragma once

#include "Global.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

std::vector<sf::Vector2<unsigned short>> get_adjacent_cells(const sf::Vector2<unsigned short>& i_cell, const gbl::MAP::Map<gbl::MAP::PathCell>& i_map);
