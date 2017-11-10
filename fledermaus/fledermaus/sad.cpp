#include "stdafx.h"
#include "sad.h"

namespace sad {

	std::string && read_until_colon(std::istream & istream)
	{
		static std::string result;

		result.clear();
		char c;
		while (istream >> c) { // read behaviour
			if (c == ':') break;
			result.push_back(c);
		}
		return std::move(result);
	}

	std::vector<std::string>&& read_string_vector_component(std::istream & istream)
	{
		static std::vector<std::string> result;

		result.clear();


		return std::move(result);
	}



	std::vector<std::string>&& Node::to_string_vector() const
	{
		static std::vector<std::string> result;
		result.clear();
		const auto& subs = sublist();
		for (auto iter = subs.cbegin(); iter != subs.cend(); ++iter) {
			result.push_back((**iter).get_value());
		}
		return std::move(result);
	}

}
