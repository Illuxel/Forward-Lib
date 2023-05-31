#pragma once

#include "fl/net/Core.hpp"

namespace fl {

    template<typename Type>
    class HttpRequestWrapper
    {
    private:
        http::request<Type> request_data_;

    public:
        HttpRequestWrapper(http::request<Type>&& request) 
            : request_data_(std::move(request)) {}



    };

    using HttpRequest = HttpRequestWrapper<http::string_body>;
}