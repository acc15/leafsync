#include "util/string.hpp"

#include <algorithm>
#include <iterator>

namespace lf {

	bool is_ascii(char c) {
		return c >= 0;
	}

	char lower(char ch) {
		return is_ascii(ch) ? static_cast<char>(std::tolower(ch)) : ch;
	}

	char upper(char ch) {
		return is_ascii(ch) ? static_cast<char>(std::toupper(ch)) : ch;
	}

	bool is_whitespace(char ch) {
		return is_ascii(ch) && std::isspace(ch);
	}

	template <std::input_iterator I, std::sentinel_for<I> S>
	size_t count_first_spaces(const I first, const S last) {
		return std::distance(first, std::find_if_not(first, last, &is_whitespace));
	}

	std::string_view rtrim(std::string_view str) {
		str.remove_suffix(count_first_spaces(str.rbegin(), str.rend()));
		return str;
	}

	std::string_view ltrim(std::string_view str) {
		str.remove_prefix(count_first_spaces(str.begin(), str.end()));
		return str;
	}

	std::string_view trim(std::string_view str) {
		return ltrim(rtrim(str));
	}

	std::string lower(std::string_view str) {
		std::string result;
		result.reserve(str.size());
		std::transform(str.begin(), str.end(), std::back_inserter(result), static_cast<char (*)(char)>(&lower));
		return result;
	}

	std::string upper(std::string_view str) {
		std::string result;
		result.reserve(str.size());
		std::transform(str.begin(), str.end(), std::back_inserter(result), static_cast<char (*)(char)>(&upper));
		return result;
	}

	void lower(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), static_cast<char (*)(char)>(&lower));
	}

	void upper(std::string& str) {
		std::transform(str.begin(), str.end(), str.begin(), static_cast<char (*)(char)>(&upper));
	}

}