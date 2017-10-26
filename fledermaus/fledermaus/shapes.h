#pragma once // make this better

#include <vector>
#include <array>
#include <typeinfo>
#include "borders.h"

namespace maus {

	using sfloat = double;
	using metric = double;

	

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
		inline const sfloat& get_in_gon() { return angle_gon;  }
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

	class Position : public std::array<metric, 2> {
	public:
		inline Position() {	this->x() = 0; this->y() = 0;	}
		inline Position(metric x, metric y) : std::array<metric, 2>{x, y} {}

		inline metric& x() {	return this->operator[](0);	}
		inline metric& y() {	return this->operator[](0);	}
	};

	class PlugType { // Derive from this class to add new Plug Type, i.e. Plug Shape (not plug size)
	public:
		bool operator == (const PlugType& rop) const {
			return typeid(*this) == typeid(rop);
		}
	};

	class StandardPlug : public PlugType {
	public:
		static const StandardPlug& instance() {
			static StandardPlug instance{};
			return instance;
		}
		inline static const StandardPlug* pInstance() { return &instance(); }
	};

	class OneLinePlug : public PlugType {
	public:
		static const OneLinePlug& instance() {
			static OneLinePlug instance{};
			return instance;
		}
		inline static const OneLinePlug* pInstance() { return &instance(); }
	};

	class Plug {
	//public:
			/* relative position to shape origin */
		Position position;
	public:
			/* direction in which the Plug points
			   from inside the shape to the outside */
		Direction direction;

			/* size of the Plug */
		sfloat size;

			/* shape or form of the Plug */
		const PlugType* plug_type;

		Plug& turn(Direction angle_gon);

		Plug(const Position& position, const Direction& direction_in_to_out, sfloat size, const PlugType* plug_type) :
			position(position), direction(direction), size(size), plug_type(plug_type) {}
	};

	class Shape {
	public:
			/* we always think of a 100*100 raster background, scaling allows us different output by << operator */
		static constexpr metric scaling_factor{ 1 };

			/* position of a Shape object (nearly the only data a general Shape has, plugs are ~ not for frontend user) */
		Position position;
			
			/* angle_gon to turn the Shape */
			/* border Types only work if using 0° 90° 180° 270° */
		Direction turning;

			/* list of plugs */
		//std::vector<Plug> plugs;

			/* inheritant class must publish the plugs in original direction orientation */
		virtual const std::vector<Plug>& get_original_plugs() const = 0;

			/* inheritant class must publish the vectors (positions) to draw the shape in origin direction */
		virtual const std::vector<Position>& get_original_coords() const = 0;

			/* takes plugs from the plugs-publishing funtion, returns a new vector with the turned plugs */
		std::vector<Plug>&& get_turned_plugs() const;

			/* takes positions from positions-publishing function, returns a new vector with the turned plugs */
		std::vector<Position>&& get_turned_coords() const;
		
			/* print out Shape */
		virtual std::ostream& operator << (std::ostream&) const = 0;

			/* returns the extension from the shape origin */
		virtual metric get_extension(int direction) const = 0;

			/* returns the Bordertype of the given directions shape border */
		virtual BorderType get_maximum_border_type(const BorderDirection&) const = 0;

		Shape(Position position/*, std::vector<Plug> plugs*/) : /*plugs(plugs),*/ position(position) {}
	};
	// What to do if inheriting:
	/*
		A shape should habe static const plugs (original ones). turned ones are created via functions.
		You have to overload get:original_plugs therefor

		A Shape shiuld have s static const 
		
	*/


	class EmptyShape : public Shape {
		static std::vector<Plug> original_plugs;
	public:
		EmptyShape(Position position) : Shape(position/*, std::vector<Plug>(0)*/) {}

		std::ostream& operator << (std::ostream& ostream) const override {
			return ostream; // just print nothing, think about it again....
		}

		metric get_extension(int) const override {
			return 0.0;
		}
		
		BorderType get_maximum_border_type(const BorderDirection&) const override {
			return BorderType::Values::SPACE; // <<< I don't know wether this is good since extensions are zero and there may be borders of neighbour shapes
		}

		const std::vector<Plug>& get_original_plugs() const override {
			return original_plugs;
		}
	};

	class Foot : public Shape {
		static const std::vector<Plug> original_plugs;
		static const std::vector<Position> original_coords;
	public:
		// origin of this Shape is the "middle" of the middle raster of the foot
		Foot(Position position) : Shape(position/*, _plugs*/) {}



	};



	/*
		Implementations
	*/
	inline Direction::Direction(const BorderDirection& border_direction) : Direction(border_direction.angle_gon) {}

};
