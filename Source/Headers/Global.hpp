#pragma once

namespace gbl
{
	struct SpriteData
	{
		unsigned short total_frames;

		std::string image_location;
		std::string name;

		sf::Rect<unsigned short> texture_box;

		sf::Sprite* sprite;

		sf::Vector2<short> origin;
	};

	namespace MAP
	{
		constexpr unsigned char CELL_SIZE = 4;
		constexpr unsigned char COLUMNS = 64;
		constexpr unsigned char ROWS = 64;

		//This cell is used for the map.
		enum class Cell
		{
			Empty,
			HelpWall,
			MugWall,
			SkeletonWall,
			TextWall,
			Wall
		};

		//This cell is used for pathfinding.
		enum class PathCell
		{
			Empty,
			Invalid,
			Visited,
			Wall
		};

		template <typename value_type = Cell>
		using Map = std::array<std::array<value_type, ROWS>, COLUMNS>;
	}

	namespace PLAYER
	{
		constexpr float MOVEMENT_SPEED = 0.125f;
	}

	namespace RAYCASTING
	{
		constexpr float FOV_HORIZONTAL = 90;
		constexpr float FOV_VERTICAL = 90;
		constexpr float MAX_VERTICAL_DIRECTION = 60;
		//Can someone give me the closest value of PI we can store in a float?
		constexpr float PI = 3.141592653589793116f;
		constexpr float RENDER_DISTANCE = 8;
	}

	namespace SCREEN
	{
		constexpr unsigned char RESIZE = 2;

		constexpr unsigned short HEIGHT = 360;
		constexpr unsigned short WIDTH = 640;

		//My PC can't handle 60 FPS so we're running it in 30.
		constexpr std::chrono::microseconds FRAME_DURATION(33334);
	}

	namespace SPRITES
	{
		constexpr float FIRE_ANIMATION_SPEED = 0.5f;
		constexpr float HAND_BOBBING_SPEED = 0.0625f;
		constexpr float HAND_SCALE = 4;

		constexpr unsigned short HAND_OFFSET_MAX = 64;
	}

	namespace STEVEN
	{
		constexpr float MAX_MOVEMENT_DISTANCE = 32;
		constexpr float MAX_MOVEMENT_SPEED = 0.25f;
		constexpr float MIN_MOVEMENT_DISTANCE = 4;
		constexpr float MIN_MOVEMENT_SPEED = 0.0625f;

		constexpr short SCREAMER_Y = -64;

		constexpr unsigned char SCREAMER_RESIZE = 16;

		constexpr unsigned short SCREAMER_MAX_OFFSET = 32;
	}

	template <typename value_type = unsigned short>
	using Position = std::pair<value_type, value_type>;
}