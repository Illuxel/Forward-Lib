#pragma once

#include "fl/utils/StringArg.hpp"

namespace Forward {

    using StringArgList = std::vector<StringArg>;

    class StringBuilder
    {
    private:
        std::optional<std::string> result_;

    public:
        StringBuilder();
        StringBuilder(std::string_view templ, StringArgList const& args = {});

        void SetTemplate(std::string_view templ);

        /**
         * Replaces all occurences with specified argument
         */
        StringBuilder& Arg(StringArg const& arg);
        /**
         * Replacec all occurences with specified list of argument
         */
        StringBuilder& Arg(StringArgList const& args);

        /**
         * Makes result as an StringArg
         * 
         * @param name represents arg name
         * @return string argument which contains name and result of string build
         */
        StringArg AsArg(std::string_view name) const;
        /**
         * Makes result as an StringArg
         *
         * @param name represents arg name
         * @param specifier places before argument name
         * @return string argument which contains name and result of string build
         */
        StringArg AsArg(std::string_view name, char specifier) const;

        std::string Data() const;

        /**
         * Clears building string
         */
        void Clear();

        // static StringBuilder MakeString(std::string_view templ);

        /**
         * Reads and wraps file data to StringBuilder
         * 
         * @param file_name which will be loaded
         * @return StringBuilder instance with loaded data from file
         */
        static StringBuilder FromFile(std::string_view file_name);

        StringBuilder& operator=(char const* str_arr);
        static StringBuilder FromFile(std::string_view file_name, StringArgList const& args);
        StringBuilder& operator=(std::string_view str);

        operator char const*() const &
        {   
            if (result_.has_value())
                return result_.value().c_str();

            return "";
        }
        operator std::string() const &
        {
            return result_.value_or("");
        }

    private:
        void BuildString(StringArg const& arg);
        void BuildString(StringArgList const& args);
    };
}