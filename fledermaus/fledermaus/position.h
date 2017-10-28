#pragma once
#include <array>

#include "basic_types.h"

namespace maus {

	class Position : public std::array<metric, 2> {
	public:
		inline Position() { this->x() = 0; this->y() = 0; }
		inline Position(metric x, metric y) : std::array<metric, 2>{x, y} {}

		inline metric& x() { return this->operator[](0); }
		inline metric& y() { return this->operator[](1); }

		inline const metric& x() const { return this->operator[](0); }
		inline const metric& y() const { return this->operator[](1); }

	};

}

inline std::ostream& operator << (std::ostream& lhs, const maus::Position& rhs) { return lhs << rhs.x() << ' ' << rhs.y(); }