#pragma once

#include <string>
#include <unordered_map>

namespace fl {
    
    class HttpUrl
    {
        bool valid_ = false;
        std::string target_;
        std::unordered_map<std::string, std::string> query_params_;

    public:
        HttpUrl();
        HttpUrl(std::string_view target);
    
        std::string Target() const;
        std::string Value(std::string_view key) const;

        bool IsTarget() const;
        bool IsValid() const;
    };

} // namespace fl
