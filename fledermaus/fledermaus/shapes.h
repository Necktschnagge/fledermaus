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

	class AppliedShape;

	class Shape {

	protected:
		std::string name;

		/* vector of component coordinates */
		std::vector<Position> coords;

		/* list of plugs (original plugs) */
		std::vector<Plug> plugs;

		std::array<metric, 4> extensions; // extensions from shape origin

		std::array<BorderType, 4> max_border_types;

		virtual void draw(std::ostream& stream, const AppliedShape& transformations) const = 0;

	public:

		Shape() {}
	};

	class PathShape : public Shape {
	
		std::vector<std::string> path_components;

	public:
		PathShape(const sad::Node& sad_component);

		void draw(std::ostream& stream, const AppliedShape& transformations) const override;
	};

	class AppliedShape {

		const Shape* orig_shape;
		Position position;
		Direction turn_angle;

	public:
		AppliedShape(const Shape& orig_shape, const Position& position, const Direction& angle) :
			orig_shape(&orig_shape), position(position), turn_angle(angle) {}

		std::vector<Plug>&& get_turned_plugs() const;

		std::vector<Position>&& get_turned_coords() const;

		friend std::ostream& operator<< (std::ostream&,const AppliedShape& shape);


	};

	/*
	class EmptyShape : public Shape {
		static std::vector<Plug> original_plugs;
	public:
		EmptyShape(Position position) : Shape(position/*, std::vector<Plug>(0)* /) {}

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

	*/

	/*class Foot : public Shape {
		static const std::vector<Plug> original_plugs;
		static const std::vector<Position> original_coords;
	public:
		// origin of this Shape is the "middle" of the middle raster of the foot
		Foot(Position position) : Shape(position) {}

		const std::vector<Plug>& get_original_plugs() const override { return original_plugs; }
		const std::vector<Position>& get_original_coords() const override { return original_coords; }

		std::ostream& operator>>(std::ostream& stream) {
			return stream;
		}
	};*/



	/*
		Implementations
	*/
	

};
