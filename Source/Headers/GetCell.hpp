#pragma once

#include "Global.hpp"
#include <SFML/System/Vector2.hpp>

gbl::MAP::PathCell get_cell(const sf::Vector2<short>& i_cell, const gbl::MAP::Map<gbl::MAP::PathCell>& i_map);
