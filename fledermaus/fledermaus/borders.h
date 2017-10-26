#pragma once

namespace maus {

	class BorderType {
		int enumerate;
	public:

		enum class Values : int {
			LSPACE = 0, // large space: more than (>) an fourth of a cell
			SPACE = 1, // space, but at most == a fourth of a cell
			SPIKE = 2, // like a hit but in a sharper way // details see somewhere else 
					   // lim<x\to 0>(b²/A) == 0. // rather (A/b²) .. I don't know ...
					   SPIKES = 5, // like SPIKE but more than 1
					   HIT = 4, // Shape "hits" the given border in only one dot
					   HITS = 8, // Shape only "hits" the border in dots, but not a infinit amount of dots
					   LINE = 16, // Shapes "hits" border in "infinite dots" (border line)
					   LINES = 32 // like line but with more than  one line which are pairwise disjunct
		};

		static constexpr int LSPACE = static_cast<int>(Values::LSPACE),
			SPACE = static_cast<int>(Values::SPACE),
			SPIKE = static_cast<int>(Values::SPIKE),
			SPIKES = static_cast<int>(Values::SPIKES),
			HIT = static_cast<int>(Values::HIT),
			HITS = static_cast<int>(Values::HITS),
			LINE = static_cast<int>(Values::LINE),
			LINES = static_cast<int>(Values::LINES);

		inline operator int() { return enumerate; }

		BorderType(int enumerate) : enumerate(enumerate) {}
		BorderType(Values value) : BorderType(static_cast<int>(value)) {}
	};

}
