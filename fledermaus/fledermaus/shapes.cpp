#include "stdafx.h"
#include "shapes.h"
//#define _USE_MATH_DEFINES
//#include <cmath>


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

	std::vector<Plug>&& Shape::get_turned_plugs() const
	{
		static std::vector<Plug> plugs{};
		plugs = get_original_plugs();
		for (Plug& plug : plugs) plug.turn(turning);
		return std::move(plugs);
	}

	const std::vector<Plug> Foot::original_plugs = std::vector<Plug>{
		maus::Plug{{0,-50},maus::Direction::NORTH,30,OneLinePlug::pInstance()}
	};
	const std::vector<Position> Foot::original_coords = std::vector<Position>{
		{-15,-50}, // go from middle up left to left end of the one and only plug
		{30,0}, // line of the plug
		{0,100}, // arc on the right side
		{65,80},
		{85,100},
		{-50,-10}, // bottom arc
		{-150,-10},
		{-200,0},
		{20,-20}, // left arc back to beginning
		{85,0},
		{85,-100}
	};
}
