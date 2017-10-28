#pragma once // make this better

#include <vector>
#include <array>
#include <typeinfo>

#include "basic_types.h"
#include "borders.h"
#include "position.h"
#include "directions.h"

namespace maus {


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
		virtual std::ostream& operator >> (std::ostream&) const = 0;

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

		A Shape shiuld have s static const original coors. turned ones aviable over inherited function.
		You should prove the original one via overloading get_original_coords
		
	*/


	class EmptyShape : public Shape {
		static std::vector<Plug> original_plugs;
	public:
		EmptyShape(Position position) : Shape(position/*, std::vector<Plug>(0)*/) {}

		std::ostream& operator >> (std::ostream& ostream) const override {
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

		const std::vector<Plug>& get_original_plugs() const override { return original_plugs; }
		const std::vector<Position>& get_original_coords() const override { return original_coords; }

		std::ostream& operator>>(std::ostream& stream) {
			return stream;
		}
	};



	/*
		Implementations
	*/
	inline Direction::Direction(const BorderDirection& border_direction) : Direction(border_direction.angle_gon) {}

};
