#pragma once

#include <string>
#include <unordered_map>

namespace fl {
    
    class HttpUrl
    {
        bool valid_;
        std::string target_, section_;
        std::unordered_map<std::string, std::string> query_params_;

    public:
        HttpUrl();
        HttpUrl(std::string_view target);
    
        std::string Target() const;
        std::string Section() const;

        std::string Value(std::string_view key) const;

        std::vector<std::string> Keys() const;
        
        bool IsTarget() const;
        bool IsValid() const;
    };

} // namespace fl
