#pragma once

#include "fl/net/Core.hpp"

namespace fl {

    template<typename Type>
    class HttpResponseWrapper
    {
    private:
        http::response<Type> request_data_;

    public:
        HttpResponseWrapper(http::response<Type>&& request)
            : request_data_(std::move(request)) {}



    };

    using HttpResponse = HttpResponseWrapper<http::string_body>;
}