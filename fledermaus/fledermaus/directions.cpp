#include "stdafx.h"

#include "directions.h"
#include <cmath>

namespace maus {

	Direction& Direction::turn(const Direction & angle_gon)
	{
		this->angle_gon = fmod(this->angle_gon + angle_gon.angle_gon, 400);
		return *this;
	}

	sfloat Direction::get_in_rad()
	{
		return sfloat(this->angle_gon * 3.14 / 400.0);// ###use pi constant from header
	}


}