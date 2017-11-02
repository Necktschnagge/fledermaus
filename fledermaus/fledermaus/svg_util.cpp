#include "stdafx.h"
#include "svg_util.h"

#include <iostream>
#include <map>
#include <exception>

namespace maus {

	class ToShortPositionVectorException : public std::exception {
		virtual const char* what() const override {
			return "Reached end of given position array.";
		}
	};

	class UnknownPathComponentException : public std::exception {
		virtual const char* what() const override {
			return "Unknown svg path component found.";
		}
	};

}

namespace {

	std::map<std::string, unsigned int> positions_for_path_component{ {"c",3}, {"l",1}, {"m",1} };
	//positions_for_path_component[std::string("l")] = 1; // not allowed, but initializer list ist best.

	inline const char* get_blank_string(unsigned int size) {
		static std::string blanks{};
		while (size > blanks.size()) {
			blanks.push_back(' ');
		}
		return blanks.c_str() + (blanks.size() - size);
	}

	/* write the next [count] Position objects beginning at [coord_iterator] to [stream].
	   AN ToSHortPositionVectorException is thrown if function reaches [coord_end_iterator] but still should write Positions to stream. */
	template <typename c_iterator>
	inline void draw_positions_ending_blank(std::ostream& stream, const unsigned int count, c_iterator& coord_iterator, const c_iterator& coord_end_iterator) {
		for (unsigned int i = 0; i < count; ++i) {
			if (coord_iterator == coord_end_iterator) {
				throw maus::ToShortPositionVectorException();
			}
			stream << *(coord_iterator++) << ' ';
		}
	}/// move this function to Position lib? -> no

	/* writes a string to [stream] containing ex. one svg path component identifier [component] and the corresponding coordinates for this component provided by [coord_iterator].
		Throws an UnknownPathComponentException if you pass an unknown [component] string.
		Throws an ToShortPositionVectorException if [coord_end_iterator] was reached but [component] requires more coordinates
	*/
	template <typename c_iterator>
	inline void draw_path_component(std::ostream& stream, const std::string & component, c_iterator& coord_iterator, const c_iterator& coord_end_iterator) {
		unsigned int cpositions;
		try {
			cpositions = positions_for_path_component.at(component);
		}
		catch (std::out_of_range) {
			throw maus::UnknownPathComponentException();
		}
		stream << component << ' ';
		try {
			draw_positions_ending_blank(stream, cpositions, coord_iterator, coord_end_iterator);
		}
		catch (...) {
			throw;
		}
	}

}

namespace maus {




	void svg::draw_path(std::ostream& stream, const Position& start_position, const std::vector<std::string>& path_components, const std::vector<Position>& coords, bool nline, unsigned int spaces) {
		// <<<< has no the prettiest arguments as it takes vectors...
		for (unsigned int i = 0; i < spaces; ++i) {
			stream << get_blank_string(spaces);
			stream << "<path d=\"M ";
			stream << start_position << ' ';
			auto position_iterator = coords.cbegin();
			for (auto component_iterator = path_components.cbegin(); component_iterator != path_components.cend(); ++component_iterator) {
				draw_path_component(stream, *component_iterator, position_iterator, coords.cend());
			}
			stream << "\" stroke-width=\"0\" fill=\"black\" stroke=\"black\" />";
		}
	}

}

