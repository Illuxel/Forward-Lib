#pragma once

#include "fl/utils/StringArg.hpp"

#include <unordered_map>

namespace fl {

    class HttpQuery
    {
        using Parameters = std::unordered_map<std::string, std::string>;
        std::optional<Parameters> params_;

    public:
        HttpQuery();
        HttpQuery(std::string_view query);

        void SetQuery(std::string_view query);

        StringArg Arg(std::string_view key) const;
        std::string_view Value(std::string_view key) const;

        std::vector<std::string> Keys() const;

        std::string ToString() const;

        std::vector<StringArg> ToArgs() const;
        std::vector<StringArg> ToArgs(char specifier) const;

        bool IsEmpty() const;
        
        bool HasKey(std::string_view key) const;
    };
} // namespace fl