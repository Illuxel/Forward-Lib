#pragma once

#include "fl/utils/StringArg.hpp"

namespace Forward {

    class StringBuilder
    {
    private:
        std::optional<std::string> result_ = std::nullopt;

    public:
        StringBuilder();
        StringBuilder(std::string_view str, StringArgList const& args = {});

        void SetTemplate(std::string_view str);

        /**
         * Replaces first occurrence with specified argument
         */
        StringBuilder& ArgFirst(StringArg const& arg);
        /**
         * Replaces first occurrence with specified list of argument
         */
        StringBuilder& ArgFirst(StringArgList const& args);

        /**
         * Replaces all occurrences with specified argument
         */
        StringBuilder& Arg(StringArg const& arg);
        /**
         * Replaces all occurrences with specified list of argument
         */
        StringBuilder& Arg(StringArgList const& args);

        std::string Data() &&;
        std::string const& Data() const&;

        /**
         * Clears building string
         */
        void Clear();

        bool IsValid() const;

        bool IsArg(StringArg const& arg) const;
        bool IsArgs(StringArgList const& args) const;

        /**
         * Reads and wraps file data to StringBuilder
         * 
         * @param file_name which will be loaded
         * @return StringBuilder instance with loaded data from file
         */
        static StringBuilder FromFile(std::string_view file_name);
        static StringBuilder FromFile(std::string_view file_name, StringArgList const& args);
;
        StringBuilder& operator=(std::string_view str);

        operator char const*() const
        {   
            if (result_.has_value())
                return result_.value().c_str();

            return "";
        }
        operator std::string const&() const&
        {
            return result_.value();
        }

    private:
        void BuildString(StringArg const& arg);
        void BuildString(StringArgList const& args);
    };
}