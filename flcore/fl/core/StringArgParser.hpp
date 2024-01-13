#pragma once

#include "fl/core/StringArg.hpp"

#include <vector>

namespace Forward {

    /**
     *  
     */
	class StringArgParser
	{
	private:
        std::string_view str_;

	public:
        StringArgParser();
        StringArgParser(std::string_view str);

        uint32_t ArgCount() const;

        /**
         * Parses string (key=value) and returns StringArg from it
         *
         * @param str string that contains argument
         * @param sep symbol that will be a separator in string
         * 
         * @return
         */
        static StringArg FromString(std::string_view str, char sep = '=', std::string_view format = "");
        /**
         * Parses string (key=value;key=value) and returns multiple StringArg
         *
         * @param str string that contains argument
         * @param
         * 
         * @return
         */
        static std::vector<StringArg> FromString(std::string_view str, char sep, char delim, std::string_view format = "");
	};
}
