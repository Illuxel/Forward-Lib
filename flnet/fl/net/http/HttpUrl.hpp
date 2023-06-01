#pragma once

#include "fl/net/http/HttpQuery.hpp"

namespace fl {
    
    class HttpUrl
    {
        bool valid_;
        std::optional<std::string> protocol_, domain_, target_, query_, section_;

    public:
        HttpUrl();
        HttpUrl(std::string const& url);
    
        std::string_view Protocol() const;
        std::string_view Domain() const;
        std::string_view Target() const;
        std::string_view Section() const;

        HttpQuery Query() const;

        bool HasProtocol() const;
        bool HasDomain() const;
        bool HasTarget() const;
        bool HasSection() const;

        bool IsValid() const;
        
    };

} // namespace fl
