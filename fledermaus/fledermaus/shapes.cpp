#include "stdafx.h"
#include "shapes.h"
//#define _USE_MATH_DEFINES
//#include <cmath>

#include "sad.h"


namespace maus {

	Plug & Plug::turn(Direction angle_gon)
	{
		this->direction.turn(angle_gon);
		metric x{ this->position.x() };
		metric y{ this->position.y() };
		this->position.x() = std::cos(angle_gon.get_in_rad())*x - std::sin(angle_gon.get_in_rad())*y;
		this->position.y() = std::sin(angle_gon.get_in_rad())*x + std::cos(angle_gon.get_in_rad())*y;
		return *this;
	}

	PathShape::PathShape(const sad::Node & sad_component)
	{
		try {
			auto iter = sad_component.scbegin();
			// name:
			name = iter->get_value();
			++iter;
			// path components:
			path_components = std::move(iter->to_string_vector());
			++iter;
			// path coords:
			for (auto jter = iter->scbegin(); jter != iter->cend(); ++jter) {
				metric x = std::stod(jter->get_value());
				++jter;
				metric y = std::stod(jter->get_value());
				coords.emplace_back(x, y);
			}
			// plugs:
			++iter;
			for (auto jter = iter->cbegin(); jter != iter->cend(); ++jter) {
				plugs.push_back(std::move(Plug(*jter)));
			}
		}
		catch (...) {
			throw;
		}
	}
	std::vector<Plug>&& AppliedShape::get_turned_plugs() const
	{
		static std::vector<Plug> temporal_plugs;
		//### do here
		++;

		return std::move(temporal_plugs);
	}
}
