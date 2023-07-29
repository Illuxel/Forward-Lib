#include "fl/net/http/HttpUrl.hpp"

#include <regex>

namespace Forward {
    
    HttpUrl::HttpUrl()
        : valid_(false) {}
    HttpUrl::HttpUrl(std::string_view url)
        : valid_(false)
    {
        auto decoded = UrlDecodeUtf8(url);
        SetUrl(decoded);
    }

    void HttpUrl::SetUrl(std::string_view url)
    {
        std::smatch match;

        std::string copy(url.data());
        std::regex urlRegex("^(?:(https?):\\/\\/)?(?:([^\\/\\?#]+))?([^\\?#]*)(?:\\?([^#]*))?(?:#(.*))?$");

        valid_ = std::regex_match(copy, match, urlRegex);

        if (!valid_) 
            return;

        protocol_.emplace(match[1].str());
        domain_.emplace(match[2].str());
        path_.emplace(match[3].str());
        query_.emplace(match[4].str());
        section_.emplace(match[5].str());
    }

    std::string HttpUrl::Protocol() const
    {
        return protocol_.value_or("");
    }
    std::string HttpUrl::Domain() const
    {
        return domain_.value_or("");
    }
    std::string HttpUrl::Path() const
    {
        return path_.value_or("");
    }
    std::string HttpUrl::Section() const
    {
        return section_.value_or("");
    }

    HttpQuery HttpUrl::Query() const
    {
        return HttpQuery(query_.value_or(""));
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
        return path_.has_value();
    }
    bool HttpUrl::HasSection() const
    {
        return section_.has_value();
    }
    
    bool HttpUrl::IsValid() const
    {
        return valid_;
    }

    bool HttpUrl::IsTargetLegal(std::string_view target)
    {
        if (target.empty() ||
            target.front() != '/' ||
            target.find("..") != std::string_view::npos)
            return false;
        
        return true;
    }

} // namespace Forward
