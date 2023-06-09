#pragma once

#include "fl/net/http/HttpQuery.hpp"

namespace fl {
    
    class HttpUrl
    {
        bool valid_;
        std::optional<std::string> protocol_, domain_, target_, query_, section_;

    public:
        HttpUrl();
        HttpUrl(std::string_view url);
    
        void SetUrl(std::string_view url);

        std::string Protocol() const;
        std::string Domain() const;
        std::string Target() const;
        std::string Section() const;

        HttpQuery Query() const;

        bool HasProtocol() const;
        bool HasDomain() const;
        bool HasTarget() const;
        bool HasSection() const;

        bool IsValid() const;
        
    };

} // namespace fl
