#pragma once

#include "fl/utils/TypedArg.hpp"

#include <string_view>

namespace Forward {

	using StringArg = TypedArg<std::string>;
	using StringArgList = std::vector<StringArg>;

	using StringViewArg = TypedArg<std::string_view>;
	using StringViewArgList = std::vector<StringViewArg>;
}
