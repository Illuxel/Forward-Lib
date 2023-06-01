#pragma once

#include <string>
#include <optional>
#include <unordered_map>

namespace fl {
    
    class HttpQuery
    {
        using Parameters = std::unordered_map<std::string, std::string>;
        std::optional<Parameters> params_;

    public:
        HttpQuery(std::string_view query);

        std::list<std::string> Keys() const;
        std::string_view Value(std::string_view key) const;

        std::string ToString() const;

        bool HasKeys() const;
        bool HasKey(std::string_view key) const;

        bool IsValueEmpty(std::string_view key) const;
    };
} // namespace fl