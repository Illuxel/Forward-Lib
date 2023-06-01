#pragma once

#include <string>
#include <optional>
#include <unordered_map>

namespace fl {
    
    class HttpUrl
    {
        using Parameters = std::unordered_map<std::string, std::string>;

        std::string target_;
        std::optional<std::string> domain_, section_, protocol_;
        std::optional<Parameters> query_params_;

    public:
        HttpUrl();
        HttpUrl(std::string_view target);
    
        std::string_view Target() const;
        std::string_view Domain() const;
        std::string_view Section() const;

        std::list<std::string> Keys() const;

        std::string_view Value(std::string_view key) const;

        bool HasDomain() const;
        bool HasTarget() const;
        bool HasSection() const;
        bool HasParameters() const;

        bool HasKey(std::string_view key) const;

        bool IsValueEmpty(std::string_view key) const;
    };

} // namespace fl
