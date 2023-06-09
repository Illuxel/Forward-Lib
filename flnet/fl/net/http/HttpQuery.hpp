#pragma once

#include "fl/utils/StringArg.hpp"

#include <string>
#include <unordered_map>

namespace fl {

    std::string UrlEncodeUtf8(std::string_view input);
    std::string UrlDecodeUtf8(std::string_view input);

    class HttpQuery
    {
        using Parameters = std::unordered_map<std::string, std::string>;
        std::optional<Parameters> params_;
        bool is_valid;
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

        size_t Size() const;

        bool IsValid() const;

        bool HasKey(std::string_view key) const;
    };
} // namespace fl