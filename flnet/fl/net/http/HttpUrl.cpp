#include "fl/net/http/HttpUrl.hpp"

#include <regex>

namespace fl {
    
    HttpUrl::HttpUrl()
        : valid_(false) {}
    HttpUrl::HttpUrl(std::string const& url)
        : valid_(false)
    {
        std::smatch match;
        std::regex urlRegex("^(?:(https?):\\/\\/)?(?:([^\\/\\?#]+))?([^\\?#]*)(?:\\?([^#]*))?(?:#(.*))?$");

        valid_ = std::regex_match(url, match, urlRegex);

        if (!valid_) return;

        protocol_ = match[1];
        domain_ = match[2];
        target_ = match[3];
        query_ = match[4];
        section_ = match[5];
    }

    std::string_view HttpUrl::Protocol() const
    {
        if (!protocol_) return "";
        return protocol_.value();
    }
    std::string_view HttpUrl::Domain() const
    {
        if (!domain_) return "";
        return domain_.value();
    }
    std::string_view HttpUrl::Target() const
    {
        if (!target_) return "";
        return target_.value();
    }
    std::string_view HttpUrl::Section() const
    {
        if (!section_) return "";
        return section_.value();
    }

    HttpQuery HttpUrl::Query() const
    {
        if (!query_) return "";
        return HttpQuery(query_.value());
    }

    bool HttpUrl::HasProtocol() const 
    {
        return protocol_.has_value();
    }
    bool HttpUrl::HasDomain() const 
    {
        return domain_.has_value();
    }
    bool HttpUrl::HasTarget() const 
    {
        return target_.has_value();
    }
    bool HttpUrl::HasSection() const
    {
        return section_.has_value();
    }
    
    bool HttpUrl::IsValid() const
    {
        return valid_;
    }

} // namespace fl
