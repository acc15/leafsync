#pragma once

#include <string>

namespace leafsync {

	struct data_location {
		std::string source = "";
		size_t line = 0;
		size_t column = 0;
	};

}