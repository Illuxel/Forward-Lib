#include "fl/net/http/HttpUrl.hpp"

#include "fl/utils/StringArg.hpp"

#include <algorithm>
#include <iterator>

namespace fl {
    
    auto UrlParametersParser(std::string& url)
    {
        std::unordered_map<std::string, std::string> params;

        while (true)
        {
            
        }
        
        return params;
    }

    HttpUrl::HttpUrl()
        : valid_(false) {}
    HttpUrl::HttpUrl(std::string_view target)
        : valid_(false) 
    {
        auto const pos = target.find('?');
        if (pos == std::string::npos)
            return;

        target_ = target.substr(0, pos);
        query_params_ = UrlParametersParser(target.substr(pos, target.size() - target_.size()));
    }

    std::string HttpUrl::Target() const
    {
        return target_;
    }
    std::string HttpUrl::Section() const
    {
        return section_;
    }
    std::string HttpUrl::Value(std::string_view key) const 
    {
        return "";
    }

    std::vector<std::string> HttpUrl::Keys() const
    {
        std::vector<std::string> keys;

        std::transform(
            query_params_.begin(), 
            query_params_.end(), 
            std::back_inserter(keys), 
            [](auto const& pair){ return pair.first; }
        );

        return keys;
    }
    
    bool HttpUrl::IsTarget() const 
    {
        return target_.empty();
    }
    bool HttpUrl::IsValid() const 
    {
        return valid_;
    }

} // namespace fl
