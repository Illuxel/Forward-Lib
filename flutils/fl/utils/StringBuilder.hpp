#pragma once

#include "fl/utils/StringArg.hpp"

namespace fl {

    class StringBuilder
    {
    private:
        std::optional<std::string> result_;

    public:
        StringBuilder();
        StringBuilder(std::string_view templ, std::vector<StringArg> const& args = {});

        void SetTemplate(std::string_view templ);

        StringBuilder& Arg(StringArg const& arg);
        StringBuilder& Arg(std::vector<StringArg> const& args);

        StringArg AsArg(std::string_view name) const;
        StringArg AsArg(std::string_view name, char specifier) const;

        std::string Data() const;

        void Clear();

        // static StringBuilder MakeString(std::string_view templ);

        static StringBuilder FromFile(std::string_view file_name);
        static StringBuilder FromFile(std::string_view file_name, std::vector<StringArg> const& args);

        StringBuilder& operator=(char const* str_arr);
        StringBuilder& operator=(std::string_view str);

        operator char const*() const 
        {   
            if (result_.has_value())
               return result_.value().c_str();

            return "";
        }
        operator std::string() const 
        {
            if (result_.has_value())
               return result_.value().c_str();

            return "";
        }

    private:
        void BuildString(StringArg const& arg);
        void BuildString(std::vector<StringArg> const& args);
    };
}