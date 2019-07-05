#pragma once // make this better

#include <vector>
#include <array>
#include <typeinfo>

#include "basic_types.h"
#include "borders.h"
#include "position.h"
#include "directions.h"
#include "sad.h"

namespace maus {

	class IllformedInputStreamException : public std::exception {
		virtual const char* what() const override {
			return "illformed string at input stream.";
		}
	};

	class Plug {
		Position position;
	public:
			/* direction in which the Plug points
			   from inside the shape to the outside */
		Direction direction;

			/* size of the Plug */
		sfloat size;

			/* shape or form of the Plug */
		std::string plug_type;

		Plug& turn(Direction angle_gon);

		Plug(const Position& position, const Direction& direction_in_to_out, sfloat size, std::string plug_type) :
			position(position), direction(direction), size(size), plug_type(plug_type) {}
		Plug(const sad::Node& sad_tree) {
			auto iter = sad_tree.scbegin();
			// Plug Position:
			{
				auto jter = iter->scbegin();
				metric pos_x = std::stod(jter->get_value());
				++jter;
				metric pos_y = std::stod(jter->get_value());
				position = Position(pos_x, pos_y);
			}
			++iter;
			// Plug Direction:
			Direction direction{ std::stod(iter->get_value()) };
			++iter;
			// plug size:
			sfloat size{ std::stod(iter->get_value()) };
			++iter;
			// plug type:
			std::string plug_type{ iter->get_value() };
		}
	};

	class Shape {

	protected:
		std::string name;

		/* vector of component coordinates */
		std::vector<Position> coords;

		/* list of plugs (original plugs) */
		std::vector<Plug> plugs;

		/* metric Extensions from origin into different directions. */
		std::array<metric, 4> extensions; // {EAST, SOUTH, WEST, NORTH } // order like in direction.h // math. positive for left hand system.

		/* BoderTypes of the 4 directions */
		std::array<BorderType, 4> max_border_types; // index mapping like in extensions

	public:

		Shape() {}

	};

	class PathShape : public Shape {
	
		std::vector<std::string> path_components;

	public:
		PathShape(const sad::Node& sad_component);

	};

	class AppliedShape {
		const Shape& orig_shape;

		// Transformation Matrix here, use Matrix Template Library!

	public:
		AppliedShape(const Shape& orig_shape) : orig_shape(orig_shape) {}

		std::vector<Plug>&& get_turned_plugs() const;

		std::vector<Position>&& get_turned_coords() const;

		friend std::ostream& operator << (std::ostream&, const AppliedShape&);


	};
	

};


std::ostream& operator << (std::ostream& stream, const maus::AppliedShape& shape) {

}