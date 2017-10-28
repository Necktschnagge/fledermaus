#pragma once

#include <ostream>
#include <vector>
#include <string>

#include "position.h"

namespace maus {

	namespace svg {

		//void draw_path_component(std::ostream& stream, const std::string& component, std::vector<Position>::const_iterator& position_iterator);
		// is now inline static in source file.

		void draw_path(std::ostream& stream, const Position& start_position, const std::vector<std::string>& path_components, const std::vector<Position>& coords, bool nline = true, unsigned int spaces = 0);

	}
}