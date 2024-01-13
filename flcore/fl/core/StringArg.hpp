#pragma once

#include "fl/core/TypedArg.hpp"

namespace Forward {

	using StringArg = TypedArg<std::string>;
	using StringArgList = std::vector<StringArg>;

	using StringViewArg = TypedArg<std::string_view>;
	using StringViewArgList = std::vector<StringViewArg>;
}
