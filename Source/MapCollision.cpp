#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "Headers/Global.hpp"

bool map_collision(const float i_x, const float i_y, const gbl::MAP::Map<>& i_map)
{
	for (unsigned char a = 0; a < 4; a++)
	{
		short x = 0;
		short y = 0;

		switch (a)
		{
			case 0:
			{
				x = static_cast<short>(floor(i_x));
				y = static_cast<short>(floor(i_y));

				break;
			}
			case 1:
			{
				x = static_cast<short>(ceil(i_x));
				y = static_cast<short>(floor(i_y));

				break;
			}
			case 2:
			{
				x = static_cast<short>(floor(i_x));
				y = static_cast<short>(ceil(i_y));

				break;
			}
			case 3:
			{
				x = static_cast<short>(ceil(i_x));
				y = static_cast<short>(ceil(i_y));
			}
		}

		if (0 <= x && 0 <= y && gbl::MAP::COLUMNS > x && gbl::MAP::ROWS > y)
		{
			if (gbl::MAP::Cell::Empty != i_map[x][y])
			{
				return 1;
			}
		}
	}

	return 0;
}
