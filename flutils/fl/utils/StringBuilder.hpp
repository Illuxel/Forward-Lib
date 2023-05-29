#pragma once

#include "fl/utils/StringArg.hpp"

namespace fl::utils {

    class StringBuilder
    {
        std::optional<std::string> result_;

    public:
        StringBuilder();
        StringBuilder(std::string_view str, std::vector<StringArg> const& args = {});

        std::string Data() const;
        void SetTemplate(std::string_view str);

        StringBuilder& Arg(StringArg const& arg);
        StringBuilder& Arg(std::vector<StringArg> const& args);

        static StringBuilder FromFile(std::string_view file_name);
        static StringBuilder FromFile(std::string_view file_name, std::vector<StringArg> const& args);

        void Clear();

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