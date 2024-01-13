#pragma once

#include "fl/web/Core.hpp"

namespace Forward::Web {
    
    class HttpUrlParser
    {
    public:
        static std::string DecodeUtf8(std::string_view input);
        static std::string EncodeUtf8(std::string_view input);
    };
} // namespace Forward::Web
