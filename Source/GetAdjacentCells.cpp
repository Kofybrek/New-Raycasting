#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Headers/Global.hpp"
#include "Headers/GetCell.hpp"

std::vector<sf::Vector2<unsigned short>> get_adjacent_cells(const sf::Vector2<unsigned short>& i_cell, const gbl::MAP::Map<gbl::MAP::PathCell>& i_map)
{
	std::array<bool, 9> valid_adjacent_cells = {};

	std::vector<sf::Vector2<unsigned short>> adjacent_cells;
	std::vector<sf::Vector2<unsigned short>> diagonal_adjacent_cells;

	for (unsigned char a = 0; a < valid_adjacent_cells.size(); a++)
	{
		gbl::MAP::PathCell cell = get_cell(sf::Vector2<short>(i_cell.x + a % 3 - 1, i_cell.y + floor(a / 3.f) - 1), i_map);

		valid_adjacent_cells[a] = gbl::MAP::PathCell::Invalid != cell && gbl::MAP::PathCell::Wall != cell;
	}

	for (unsigned char a = 0; a < 3; a++)
	{
		for (unsigned char b = 0; b < 3; b++)
		{
			if ((1 != a || 1 != b) && 1 == valid_adjacent_cells[b + 3 * a])
			{
				if (abs(a - 1) == abs(b - 1))
				{
					//When a potential adjacent cell is located diagonally, we need to check 2 additional cells as well.
					if (1 == valid_adjacent_cells[1 + 3 * a] && 1 == valid_adjacent_cells[3 + b])
					{
						diagonal_adjacent_cells.push_back(sf::Vector2<unsigned short>(b + i_cell.x - 1, a + i_cell.y - 1));
					}
				}
				else
				{
					adjacent_cells.push_back(sf::Vector2<unsigned short>(b + i_cell.x - 1, a + i_cell.y - 1));
				}
			}
		}
	}

	//I just realized that we don't need to put diagonal adjacent cells separately since we're not using the BFS algorithm.
	//I wish I could fix it but I don't want to.
	//...
	//Why don't you fix it instead, huh?
	//Yeah, this will be your homework.
	//I'm your teacher now.
	adjacent_cells.insert(adjacent_cells.end(), diagonal_adjacent_cells.begin(), diagonal_adjacent_cells.end());

	return adjacent_cells;
}
