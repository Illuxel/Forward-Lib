#pragma once

#include "fl/net/http/HttpQuery.hpp"

namespace Forward {
    
    class HttpUrl
    {
    private:
        std::string protocol_, domain_, path_, query_, section_;
        bool is_valid = false;

    public:
        HttpUrl();
        HttpUrl(std::string_view url);
    
        void SetUrl(std::string_view url);

        std::string Protocol() const;
        std::string Domain() const;
        std::string Path() const;
        std::string Section() const;

        HttpQuery Query() const;

        bool HasProtocol() const;
        bool HasDomain() const;
        bool HasTarget() const;
        bool HasSection() const;

        bool IsValid() const;
        
        static bool IsPathLegal(std::string_view path);
    };

} // namespace Forward
