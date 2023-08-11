#include "fl/net/http/HttpUrl.hpp"

#include <regex>

namespace Forward {
    
    HttpUrl::HttpUrl() {}
    HttpUrl::HttpUrl(std::string_view url)
    {
        if (url.empty())
            return;

        std::string decoded = UrlDecodeUtf8(url);
        SetUrl(decoded);
    }

    void HttpUrl::SetUrl(std::string_view url)
    {
        std::smatch match;

        std::string copy(url.data());
        std::regex urlRegex("^(?:(https?):\\/\\/)?(?:([^\\/\\?#]+))?([^\\?#]*)(?:\\?([^#]*))?(?:#(.*))?$");

        is_valid = std::regex_match(copy, match, urlRegex);

        if (!is_valid) return;

        protocol_ = match[1].str();
        domain_ = match[2].str();
        path_ = match[3].str();
        query_ = match[4].str();
        section_ = match[5].str();
    }

    std::string HttpUrl::Protocol() const
    {
        return protocol_;
    }
    std::string HttpUrl::Domain() const
    {
        return domain_;
    }
    std::string HttpUrl::Path() const
    {
        return path_;
    }
    std::string HttpUrl::Section() const
    {
        return section_;
    }

    HttpQuery HttpUrl::Query() const
    {
        return HttpQuery(query_);
    }

    bool HttpUrl::HasProtocol() const 
    {
        return protocol_.empty();
    }
    bool HttpUrl::HasDomain() const 
    {
        return domain_.empty();
    }
    bool HttpUrl::HasTarget() const 
    {
        return path_.empty();
    }
    bool HttpUrl::HasSection() const
    {
        return section_.empty();
    }
    
    bool HttpUrl::IsValid() const
    {
        return is_valid;
    }

    bool HttpUrl::IsPathLegal(std::string_view path)
    {
        if (path.empty() ||
            path.front() != '/' ||
            path.find("..") != std::string_view::npos)
            return false;
        
        return true;
    }

} // namespace Forward
