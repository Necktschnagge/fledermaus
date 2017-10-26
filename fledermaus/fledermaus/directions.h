#pragma once

#include "basic_types.h"

namespace maus {
	class BorderDirection;

	class Direction { // for angles. 
		sfloat angle_gon; // in right angle == 100, 0 is to east, 100 is to north
	public:
		static constexpr int EAST{ 0 };
		static constexpr int NORTH{ 100 };
		static constexpr int WEST{ 200 };
		static constexpr int SOUTH{ 300 };

		Direction() : angle_gon(0.0) {}
		Direction(int integer_angle) : angle_gon(integer_angle % 400) {}
		inline Direction(const BorderDirection& border_direction);

		Direction& turn(const Direction& angle_gon);

		sfloat get_in_rad();
		inline const sfloat& get_in_gon() { return angle_gon; }
	};

	class BorderDirection { // like direction but only for 0, 100, 200, 300 
		int angle_gon;
	public:
		enum class EnumBorderDirections : int { EAST = 0, NORTH = 100, WEST = 200, SOUTH = 300 };

		static constexpr int EAST{ 0 };
		static constexpr int NORTH{ 100 };
		static constexpr int WEST{ 200 };
		static constexpr int SOUTH{ 300 };

		BorderDirection(EnumBorderDirections direction) : angle_gon(static_cast<int>(direction)) {}
		BorderDirection(int angle_gon) : angle_gon((((angle_gon + 50) / 100) % 4) * 100) {}
		BorderDirection(const Direction& direction) : angle_gon(
			0//fmod(direction.get_in_gon(), 100)
		) {} // round fmod, etc the direction to the nearest BDir. ###

		friend Direction;
	};

}