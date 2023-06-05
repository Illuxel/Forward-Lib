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

        protocol_.emplace(match[1].str());
        domain_.emplace(match[2].str());
        target_.emplace(match[3].str());
        query_.emplace(match[4].str());
        section_.emplace(match[5].str());
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
        if (!query_) 
            return HttpQuery("");

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
